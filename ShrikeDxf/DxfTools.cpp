#include "DxfTools.h"
#include "DxfManager.h"

CDxfTools::CDxfTools(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent)
    : QObject(parent)
    , m_pData(pData)
    , m_pScene(pScene)
{
}

CDxfTools::~CDxfTools()
{
}

const QString& CDxfTools::GetCurrentLayer() const
{
    auto* pMgr = qobject_cast<CDxfManager*>(parent());
    if (pMgr)
        return pMgr->GetCurrentLayer();

    // 保底值
    static const QString s_defaultLayer = QStringLiteral("0");
    return s_defaultLayer;
}

void CDxfTools::SetMouseStatus(enumMouseStateInView mouseState)
{
    if (m_pScene)
        m_pScene->ClearPreview();
    m_eCurrentTool = mouseState;
    m_step = 0;
    m_vecPolyPoints.clear();
}

void CDxfTools::OnMouseMove(QPointF scenePos)
{
    if (!m_pScene) return;

    if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Point)                    // 画点预览
    {
        m_pScene->ClearPreview();
        m_pScene->AddPreviewPoint(scenePos);
    }  
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Line)                // 画线预览
    {
        if (m_step == 1)
        {
            // 已选起点,画从起点到鼠标位置的预览线
            m_pScene->ClearPreview();
            m_pScene->AddPreviewLine(m_ptStart, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_CircleCenterRadius)  // 圆心-半径画圆预览
    {
        if (m_step == 1)
        {
            qreal radius = QLineF(m_ptStart, scenePos).length();
            m_pScene->ClearPreview();
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_CircleDiameter)      // 直径画圆预览
    {
        if (m_step == 1)
        {
            QPointF center((m_ptStart.x() + scenePos.x()) / 2.0,
                (m_ptStart.y() + scenePos.y()) / 2.0);
            qreal radius = QLineF(m_ptStart, scenePos).length() / 2.0;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewCircle(center, radius);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_ArcCenterEndpoint)    // 圆心-端点画弧预览
    {
        if (m_step == 1)
        {
            // 已选圆心,画从圆心到鼠标位置的半径线 + 预览圆
            m_pScene->ClearPreview();
            qreal radius = QLineF(m_ptStart, scenePos).length();
            m_pScene->AddPreviewLine(m_ptStart, scenePos);
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
        else if (m_step == 2)
        {
            // 已选起点,画从起点到鼠标位置的弧预览
            m_pScene->ClearPreview();
            qreal radius = QLineF(m_ptStart, m_ptMid).length();
            qreal startAngle = atan2(m_ptMid.y() - m_ptStart.y(), m_ptMid.x() - m_ptStart.x());
            qreal endAngle = atan2(scenePos.y() - m_ptStart.y(), scenePos.x() - m_ptStart.x());
            m_pScene->AddPreviewArc(m_ptStart, radius, startAngle, endAngle);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_ArcThreePoints)       // 三点画弧预览
    {
        if (m_step == 1)
        {
            // 已选第一点,画从第一点到鼠标位置的预览线
            m_pScene->ClearPreview();
            m_pScene->AddPreviewLine(m_ptStart, scenePos);
        }
        else if (m_step == 2)
        {
            // 已有两点,用三点计算弧并预览
            m_pScene->ClearPreview();

            QPointF center;
            qreal radius, startAngle, endAngle;
            if (ThreePointsToArc(m_ptStart, m_ptMid, scenePos, center, radius, startAngle, endAngle))
            {
                m_pScene->AddPreviewArc(center, radius, startAngle, endAngle);
            }
            else
            {
                // 三点共线,画一条预览线
                m_pScene->AddPreviewLine(m_ptStart, scenePos);
            }
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Polyline)          // 多段线预览
    {
        if (m_step >= 1 && !m_vecPolyPoints.isEmpty())
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPolyline(m_vecPolyPoints, scenePos);
        }
    }
}

void CDxfTools::OnGraphicsViewLeftClick(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;

    switch (m_eCurrentTool)
    {
    case enumMouseStateInView::enumMouseState_None:
        break;

    case enumMouseStateInView::enumMouseState_Point:                    // 画点
    {
        std::string layerName = GetCurrentLayer().toStdString();

        EntityPoint pt;
        pt.prop.layer = layerName;
        pt.prop.color = 256;
        pt.point.setX(scenePos.x());
        pt.point.setY(scenePos.y());

        m_pData->AddEntity(layerName, pt);

        m_pScene->ClearPreview();
        m_pScene->DxfDraw(m_pData->GetLayers());
        break;
    }
    case enumMouseStateInView::enumMouseState_Line:                      // 画线
    {
        if (m_step == 0)
        {
            // 第一步:记录起点
            m_ptStart = scenePos;
            m_step = 1;
            // 在起点处画一个预览点,提示已选起点
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:终点点击,创建实际线段
            std::string layerName = GetCurrentLayer().toStdString();
            EntityLine line;
            line.prop.layer = layerName;
            line.prop.color = 256;
            line.prop.visible = true;
            line.startPoint.setX(m_ptStart.x());
            line.startPoint.setY(m_ptStart.y());
            line.endPoint.setX(scenePos.x());
            line.endPoint.setY(scenePos.y());
            m_pData->AddEntity(layerName, line);
            // 清理预览,重绘场景
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_CircleCenterRadius:       // 圆心-半径画圆
    {
        if (m_step == 0)
        {
            // 第一步:记录圆心
            m_ptStart = scenePos;
            m_step = 1;
            // 在圆心处画预览十字
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:点击确定半径,创建实际圆
            std::string layerName = GetCurrentLayer().toStdString();
            qreal radius = QLineF(m_ptStart, scenePos).length();
            EntityCircle circle;
            circle.prop.layer = layerName;
            circle.prop.color = 256;
            circle.prop.visible = true;
            circle.center.setX(m_ptStart.x());
            circle.center.setY(m_ptStart.y());
            circle.radius = radius;
            m_pData->AddEntity(layerName, circle);
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_CircleDiameter:           // 直径画圆 
    {
        if (m_step == 0)
        {
            // 第一步:记录直径起点
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:点击确定直径终点
            std::string layerName = GetCurrentLayer().toStdString();
            QPointF center((m_ptStart.x() + scenePos.x()) / 2.0,
                (m_ptStart.y() + scenePos.y()) / 2.0);
            qreal radius = QLineF(m_ptStart, scenePos).length() / 2.0;
            EntityCircle circle;
            circle.prop.layer = layerName;
            circle.prop.color = 256;
            circle.prop.visible = true;
            circle.center.setX(center.x());
            circle.center.setY(center.y());
            circle.radius = radius;
            m_pData->AddEntity(layerName, circle);
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_ArcCenterEndpoint:        // 圆心+起点画弧
    {
        if (m_step == 0)
        {
            // 第一步:点击圆心
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:点击起点,确定半径和起始角度
            m_ptMid = scenePos;
            m_step = 2;
            // 在起点画预览十字,并绘制半径线和圆
            qreal radius = QLineF(m_ptStart, m_ptMid).length();
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptMid);
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
        else if (m_step == 2)
        {
            // 第三步:点击终点,确定结束角度,创建弧
            std::string layerName = GetCurrentLayer().toStdString();
            qreal radius = QLineF(m_ptStart, m_ptMid).length();
            qreal startAngle = atan2(m_ptMid.y() - m_ptStart.y(),
                m_ptMid.x() - m_ptStart.x());
            qreal endAngle = atan2(scenePos.y() - m_ptStart.y(),
                scenePos.x() - m_ptStart.x());
            EntityArc arc;
            arc.prop.layer = layerName;
            arc.prop.color = 256;
            arc.prop.visible = true;
            arc.center.setX(m_ptStart.x());
            arc.center.setY(m_ptStart.y());
            arc.radius = radius;
            arc.startAngle = startAngle;
            arc.endAngle = endAngle;
            arc.isCCW = true;   // 默认逆时针
            m_pData->AddEntity(layerName, arc);
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_ArcThreePoints:           // 三点画弧
    {
        if (m_step == 0)
        {
            // 第一步:点击第一点(起点)
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:点击第二点(弧上一点)
            m_ptMid = scenePos;
            m_step = 2;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptMid);
        }
        else if (m_step == 2)
        {
            // 第三步:点击第三点终点,计算并创建弧
            std::string layerName = GetCurrentLayer().toStdString();
            QPointF center;
            qreal radius, startAngle, endAngle;
            if (ThreePointsToArc(m_ptStart, m_ptMid, scenePos, center, radius, startAngle, endAngle))
            {
                // 确定方向:检查中间点是否在从起点到终点的逆时针路径上
                bool isCCW = IsArcCCW(m_ptStart, m_ptMid, scenePos, center);
                EntityArc arc;
                arc.prop.layer = layerName;
                arc.prop.color = 256;
                arc.prop.visible = true;
                arc.center.setX(center.x());
                arc.center.setY(center.y());
                arc.radius = radius;
                arc.startAngle = startAngle;
                arc.endAngle = endAngle;
                arc.isCCW = isCCW;
                m_pData->AddEntity(layerName, arc);
            }
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_Polyline:
    {
        if (m_step == 0)
        {
            // 第一步:点击第一个点
            m_vecPolyPoints.clear();
            m_vecPolyPoints.append(scenePos);
            m_step = 1;
            // 在起点画预览十字
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(scenePos);
        }
        else
        {
            // 后续步骤:添加新顶点
            m_vecPolyPoints.append(scenePos);
            m_step++;
            // 更新预览(画所有已确定的线段 + 新顶点十字)
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPolyline(m_vecPolyPoints, scenePos);
        }
        break;
    }

    default:
        break;
    }
}

void CDxfTools::OnGraphicsViewRightClick(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;
    switch (m_eCurrentTool)
    {
    case enumMouseStateInView::enumMouseState_Polyline:
    {
        // 右键:完成多段线
        if (m_step >= 1 && m_vecPolyPoints.size() >= 2)
        {
            FinishPolyline();
        }
        else
        {
            // 点数不够，直接取消
            CancelPolyline();
        }
        break;
    }
    default:
        
        break;
    }
}



void CDxfTools::FinishPolyline()
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_Polyline)
        return;
    if (m_vecPolyPoints.size() < 2)
    {
        // 点数不够，取消
        CancelPolyline();
        return;
    }

    std::string layerName = GetCurrentLayer().toStdString();

    // 创建 LWPolyline(轻量多段线)
    EntityLWPolyline poly;
    poly.prop.layer = layerName;
    poly.prop.color = 256;
    poly.prop.visible = true;
    poly.flags = 0;              // 不闭合

    for (const auto& pt : m_vecPolyPoints)
    {
        PolylineVertex2D vertex;
        vertex.point.setX(pt.x());
        vertex.point.setY(pt.y());
        vertex.bulge = 0.0;     // 直线段
        poly.vecVertices.push_back(vertex);
    }

    m_pData->AddEntity(layerName, poly);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());

    // 重置
    m_vecPolyPoints.clear();
    m_step = 0;
}

void CDxfTools::CancelPolyline()
{
    if (m_pScene)
        m_pScene->ClearPreview();
    m_vecPolyPoints.clear();
    m_step = 0;
}
