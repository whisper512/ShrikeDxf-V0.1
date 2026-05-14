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
    if (mouseState != enumMouseStateInView::enumMouseState_None)
        ClearSelection();

    if (m_pScene)
    {
        m_pScene->ClearPreview();
    }
    m_eCurrentTool = mouseState;
    m_step = 0;
    m_vecPolyPoints.clear();
    m_vecSplinePoints.clear();

}

void CDxfTools::OnMouseMove(QPointF scenePos)
{
    if (!m_pScene) return;

    if (m_nDragGripIndex >= 0 && m_bEntitySelected)
    {
        return;
    }

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
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Polyline)              // 多段线预览
    {
        if (m_step >= 1 && !m_vecPolyPoints.isEmpty())
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPolyline(m_vecPolyPoints, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_EllipseCenterRadius)   // 中心-半径画椭圆预览
    {
        if (m_step == 1)
        {
            // 已选中心,画中心到鼠标的预览线 + 预览圆(临时)
            m_pScene->ClearPreview();
            qreal radius = QLineF(m_ptStart, scenePos).length();
            m_pScene->AddPreviewLine(m_ptStart, scenePos);
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
        else if (m_step == 2)
        {
            // 已选第一轴端点,画完整椭圆预览
            m_pScene->ClearPreview();
            double majorLen = QLineF(m_ptStart, m_ptMid).length();
            double mouseDist = QLineF(m_ptStart, scenePos).length();
            double ratio = (majorLen > 1e-10) ? (mouseDist / majorLen) : 0.1;
            if (ratio > 1.0) ratio = 1.0;   // 短轴不能超过长轴
            if (ratio < 0.01) ratio = 0.01;
            m_pScene->AddPreviewEllipse(m_ptStart, m_ptMid, ratio);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Rectangle)             // 矩形预览 
    {
        if (m_step == 1)
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewRectangle(m_ptStart, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_SplineFitPoint)        // 拟合点样条预览
    {
        if (m_step >= 1 && !m_vecSplinePoints.isEmpty())
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewSplineFit(m_vecSplinePoints, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_SplineControlPoint)    // 控制点样条预览
    {
        if (m_step >= 1 && !m_vecSplinePoints.isEmpty())
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewSplineControl(m_vecSplinePoints, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Text)                   // 文本预览
    {
        if (m_step == 1)
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewTextRect(m_ptStart, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_MText)                  // 多行文本预览
    {
        if (m_step == 1)
        {
            m_pScene->ClearPreview();
            m_pScene->AddPreviewTextRect(m_ptStart, scenePos);
        }
    }
}

void CDxfTools::OnGraphicsViewLeftClick(QPointF scenePos)
{
    // 如果在拖拽手柄中,忽略
    if (m_nDragGripIndex >= 0)
        return;

    if (!m_pData || !m_pScene) 
        return;

    switch (m_eCurrentTool)
    {
    case enumMouseStateInView::enumMouseState_None:
        HitTest(scenePos);                                              // 判断有没有选中scene中的图元
        return;
    case enumMouseStateInView::enumMouseState_Point:                    // 画点
    {
        std::string layerName = GetCurrentLayer().toStdString();

        EntityPoint pt;
        pt.prop.layer = layerName;
        pt.prop.color = 256;
        pt.point = scenePos;
        m_pData->AddEntity(layerName, pt);
        m_pScene->ClearPreview();
        m_pScene->DxfDraw(m_pData->GetLayers());
        break;
    }
    case enumMouseStateInView::enumMouseState_Line:                      // 画线
    {
        if (m_step == 0)                                                 // 第一步:记录起点
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)                                            // 第二步:终点点击,创建实际线段
        {
            std::string layerName = GetCurrentLayer().toStdString();
            EntityLine line;
            line.prop.layer = layerName;
            line.prop.color = 256;
            line.prop.visible = true;
            line.startPoint = m_ptStart;
            line.endPoint = scenePos;
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
        if (m_step == 0)                                                // 第一步:记录圆心
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)                                           // 第二步:点击确定半径,创建实际圆
        {
            std::string layerName = GetCurrentLayer().toStdString();
            qreal radius = QLineF(m_ptStart, scenePos).length();
            EntityCircle circle;
            circle.prop.layer = layerName;
            circle.prop.color = 256;
            circle.prop.visible = true;
            circle.center = m_ptStart;
            circle.radius = radius;
            m_pData->AddEntity(layerName, circle);
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            m_step = 0;
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_CircleDiameter:            // 直径画圆 
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
            circle.center = center;
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
            m_ptStart = scenePos;                                       // 第一步:点击圆心
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            m_ptMid = scenePos;                                         // 第二步:点击起点,确定半径和起始角度
            m_step = 2;
            qreal radius = QLineF(m_ptStart, m_ptMid).length();
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptMid);
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
        else if (m_step == 2)                                           // 第三步:点击终点,确定结束角度,创建弧
        {   
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
            arc.center = m_ptStart;
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
            m_ptStart = scenePos;                                       // 第一步:点击第一点(起点)
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            m_ptMid = scenePos;                                         // 第二步:点击第二点(弧上一点)
            m_step = 2;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptMid);
        }
        else if (m_step == 2)
        {
            std::string layerName = GetCurrentLayer().toStdString();    // 第三步:点击第三点终点,计算并创建弧
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
                arc.center = center;
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
    case enumMouseStateInView::enumMouseState_Polyline:                // 多段线
    {
        if (m_step == 0)
        {
            m_vecPolyPoints.clear();                                   // 第一步:点击第一个点
            m_vecPolyPoints.append(scenePos);
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(scenePos);
        }
        else                                                           // 后续步骤:添加新顶点   
        {
            m_vecPolyPoints.append(scenePos);
            m_step++;
            // 更新预览(画所有已确定的线段 + 新顶点十字)
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPolyline(m_vecPolyPoints, scenePos);
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_EllipseCenterRadius:      // 圆心+半径画椭圆
    {
        if (m_step == 0)                                                // 第一步:点击中心
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)                                            // 第二步:点击长轴端点
        {
            m_ptMid = scenePos;
            m_step = 2;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptMid);
        }
        else if (m_step == 2)                                            // 第三步:点击确定短轴比例
        {
            FinishEllipse(scenePos);
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_Rectangle:                  // 矩形(两点对角)
    {
        if (m_step == 0)                                                  // 第一步：点击第一个角点
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)                                             // 第二步:点击对角点，完成矩形
        {
            FinishRectangle(scenePos);
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_SplineFitPoint:             // 拟合点样条
    {
        if (m_step == 0)
        {
            m_vecSplinePoints.clear();
            m_vecSplinePoints.append(scenePos);
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(scenePos);
        }
        else
        {
            m_vecSplinePoints.append(scenePos);
            m_step++;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewSplineFit(m_vecSplinePoints, scenePos);
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_SplineControlPoint:           // 控制点样条
    {
        if (m_step == 0)
        {
            m_vecSplinePoints.clear();
            m_vecSplinePoints.append(scenePos);
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(scenePos);
        }
        else
        {
            m_vecSplinePoints.append(scenePos);
            m_step++;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewSplineControl(m_vecSplinePoints, scenePos);
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_Text:                         // 单行文本
    {
        if (m_step == 0)                                                    // 第一步:点击插入点
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            FinishText(scenePos);                                           // 第二步:点击确定文本区域，创建文本
        }
        break;
    }
    case enumMouseStateInView::enumMouseState_MText:                        // 多行文本
    {
        if (m_step == 0)                                                    // 第一步:点击插入点
        {
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)                                               // 第二步:点击确定文本宽度，创建多行文本
        {
            FinishMText(scenePos);
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
    case enumMouseStateInView::enumMouseState_Polyline:                   // 右键:完成多段线
    {
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
    case enumMouseStateInView::enumMouseState_SplineFitPoint:
    {
        if (m_step >= 1 && m_vecSplinePoints.size() >= 2)
            FinishSplineFit();
        else
            CancelSpline();
        break;
    }
    case enumMouseStateInView::enumMouseState_SplineControlPoint:
    {
        if (m_step >= 1 && m_vecSplinePoints.size() >= 2)
            FinishSplineControl();
        else
            CancelSpline();
        break;
    }
    default:
        
        break;
    }
}

void CDxfTools::OnGraphicsViewLeftPress(QPointF scenePos)
{
 
}
void CDxfTools::OnGraphicsViewLeftRelease(QPointF scenePos)
{
    
}

void CDxfTools::OnGripDragStarted(int gripIndex)
{
}

void CDxfTools::OnGripDragged(int gripIndex, QPointF newPos)
{
}

void CDxfTools::OnGripDragFinished(int gripIndex, QPointF finalPos)
{
}



void CDxfTools::FinishPolyline()
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_Polyline)
        return;
    if (m_vecPolyPoints.size() < 2)
    {
        // 点数不够,取消
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
        vertex.point = pt;
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

void CDxfTools::FinishEllipse(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_EllipseCenterRadius)
        return;

    std::string layerName = GetCurrentLayer().toStdString();

    double majorLen = QLineF(m_ptStart, m_ptMid).length();
    double mouseDist = QLineF(m_ptStart, scenePos).length();
    double ratio = (majorLen > 1e-10) ? (mouseDist / majorLen) : 0.1;
    if (ratio > 1.0) ratio = 1.0;
    if (ratio < 0.01) ratio = 0.01;

    EntityEllipse ellipse;
    ellipse.prop.layer = layerName;
    ellipse.prop.color = 256;
    ellipse.prop.visible = true;
    ellipse.center = m_ptStart;
    ellipse.majorAxisEndpoint.setX(m_ptMid.x() - m_ptStart.x());
    ellipse.majorAxisEndpoint.setY(m_ptMid.y() - m_ptStart.y());
    ellipse.ratio = ratio;
    ellipse.startParam = 0.0;
    ellipse.endParam = 2.0 * M_PI;

    m_pData->AddEntity(layerName, ellipse);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());
    m_step = 0;
}

void CDxfTools::FinishRectangle(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_Rectangle)
        return;

    std::string layerName = GetCurrentLayer().toStdString();

    qreal x1 = m_ptStart.x(), y1 = m_ptStart.y();
    qreal x2 = scenePos.x(), y2 = scenePos.y();

    EntityLWPolyline poly;
    poly.prop.layer = layerName;
    poly.prop.color = 256;
    poly.prop.visible = true;
    poly.flags = 1;  // 闭合

    // 四个顶点：左下/右下/右上/左上(或任意顺时针/逆时针顺序)
    PolylineVertex2D v1, v2, v3, v4;
    v1.point.setX(x1);  v1.point.setY(y1);  v1.bulge = 0.0;
    v2.point.setX(x2);  v2.point.setY(y1);  v2.bulge = 0.0;
    v3.point.setX(x2);  v3.point.setY(y2);  v3.bulge = 0.0;
    v4.point.setX(x1);  v4.point.setY(y2);  v4.bulge = 0.0;

    poly.vecVertices.push_back(v1);
    poly.vecVertices.push_back(v2);
    poly.vecVertices.push_back(v3);
    poly.vecVertices.push_back(v4);

    m_pData->AddEntity(layerName, poly);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());
    m_step = 0;
}


void CDxfTools::FinishSplineFit()
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_SplineFitPoint)
        return;
    if (m_vecSplinePoints.size() < 2)
    {
        CancelSpline();
        return;
    }

    std::string layerName = GetCurrentLayer().toStdString();

    EntitySpline spline;
    spline.prop.layer = layerName;
    spline.prop.color = 256;
    spline.prop.visible = true;
    spline.degree = 3;

    // 拟合点
    for (const auto& pt : m_vecSplinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.fitPoints.push_back(vertext);
    }

    
    for (const auto& pt : m_vecSplinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.controlPoints.push_back(vertext);
    }

    // 生成 clamped 节点向量
    int n = (int)spline.controlPoints.size();
    int k = spline.degree;
    int m = n + k + 1;
    spline.knots.resize(m);
    for (int i = 0; i < m; ++i)
    {
        if (i <= k)
            spline.knots[i] = 0.0;
        else if (i >= n)
            spline.knots[i] = 1.0;
        else
            spline.knots[i] = static_cast<double>(i - k) / (n - k);
    }

    m_pData->AddEntity(layerName, spline);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());

    m_vecSplinePoints.clear();
    m_step = 0;
}

void CDxfTools::FinishSplineControl()
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_SplineControlPoint)
        return;
    if (m_vecSplinePoints.size() < 2)
    {
        CancelSpline();
        return;
    }

    std::string layerName = GetCurrentLayer().toStdString();

    EntitySpline spline;
    spline.prop.layer = layerName;
    spline.prop.color = 256;
    spline.prop.visible = true;
    spline.degree = 3;

    // 控制点
    for (const auto& pt : m_vecSplinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.controlPoints.push_back(vertext);
    }

    // 生成 clamped 节点向量
    int n =(int)spline.controlPoints.size();
    int k = spline.degree;
    int m = n + k + 1;
    spline.knots.resize(m);
    for (int i = 0; i < m; ++i)
    {
        if (i <= k)
            spline.knots[i] = 0.0;
        else if (i >= n)
            spline.knots[i] = 1.0;
        else
            spline.knots[i] = static_cast<double>(i - k) / (n - k);
    }

    m_pData->AddEntity(layerName, spline);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());

    m_vecSplinePoints.clear();
    m_step = 0;
}

void CDxfTools::CancelSpline()
{
    if (m_pScene)
        m_pScene->ClearPreview();
    m_vecSplinePoints.clear();
    m_step = 0;
}

void CDxfTools::FinishText(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_Text)
        return;

    std::string layerName = GetCurrentLayer().toStdString();

    // 计算文本高度 = 矩形高度的一半
    double height = std::abs(scenePos.y() - m_ptStart.y());
    if (height < 1.0) height = 3.0;

    EntityText text;
    text.prop.layer = layerName;
    text.prop.color = 256;
    text.prop.visible = true;
    text.text = "Text";
    text.style = "STANDARD";
    text.insertPoint = m_ptStart;
    text.height = height;
    text.rotation = 0.0;
    text.alignH = 0;  // Left
    text.alignV = 0;  // Middle

    m_pData->AddEntity(layerName, text);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());
    m_step = 0;
}

void CDxfTools::FinishMText(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;
    if (m_eCurrentTool != enumMouseStateInView::enumMouseState_MText)
        return;

    std::string layerName = GetCurrentLayer().toStdString();

    // 计算文本高度和宽度
    double height = std::abs(scenePos.y() - m_ptStart.y());
    if (height < 1.0) height = 3.0;

    double width = std::abs(scenePos.x() - m_ptStart.x());
    if (width < 1.0) width = 20.0;

    EntityMText mtext;
    mtext.prop.layer = layerName;
    mtext.prop.color = 256;
    mtext.prop.visible = true;
    mtext.text = "MText";
    mtext.style = "STANDARD";
    mtext.insertPoint = m_ptStart;
    mtext.height = height;
    mtext.rotation = 0.0;
    mtext.attachPoint = 1;   // TopLeft
    mtext.xAxisDir.setX(width);
    mtext.xAxisDir.setY(0);

    m_pData->AddEntity(layerName, mtext);

    m_pScene->ClearPreview();
    m_pScene->DxfDraw(m_pData->GetLayers());
    m_step = 0;
}


void CDxfTools::HitTest(QPointF scenePos)
{
    // 清除之前选中
    ClearSelection();

    if (!m_pData || !m_pScene) return;

    const auto& layers = m_pData->GetLayers();

    // 用像素转场景坐标算容差
    double threshold = 15.0 / m_pScene->GetScale();
    if (threshold < 0.5) threshold = 0.5;   // 最小 0.5 场景单位

    QString hitLayer;
    int hitIndex = -1;
    double minDist = threshold;

    // 遍历所有可见图层（后画的在上层用 reverse）
    for (auto it = layers.rbegin(); it != layers.rend(); ++it)
    {
        if (!it->second.isVisible) continue;

        const auto& entities = it->second.entities;
        for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i)
        {
            const auto& entity = entities[i];
            EntityType type = GetEntityType(entity);
            double dist = -1.0;

            switch (type)
            {
            case EntityType::Point:
            {
                const auto& pt = std::get<EntityPoint>(entity);
                dist = QLineF(scenePos, QPointF(pt.point.x(), pt.point.y())).length();
                break;
            }
            case EntityType::Line:
            {
                const auto& line = std::get<EntityLine>(entity);
                dist = pointToSegmentDist(scenePos,
                    QPointF(line.startPoint.x(), line.startPoint.y()),
                    QPointF(line.endPoint.x(), line.endPoint.y()));
                break;
            }
            case EntityType::Circle:
            {
                const auto& circle = std::get<EntityCircle>(entity);
                double d = QLineF(scenePos, QPointF(circle.center.x(), circle.center.y())).length();
                dist = std::abs(d - circle.radius);
                break;
            }
            case EntityType::Arc:
            {
                const auto& arc = std::get<EntityArc>(entity);
                double d = QLineF(scenePos, QPointF(arc.center.x(), arc.center.y())).length();
                dist = std::abs(d - arc.radius);
                break;
            }
            case EntityType::Ellipse:
            {
                const auto& ellipse = std::get<EntityEllipse>(entity);
                // 用椭圆方程近似计算点到椭圆边的距离
                double dx = scenePos.x() - ellipse.center.x();
                double dy = scenePos.y() - ellipse.center.y();
                double majorLen = std::sqrt(
                    ellipse.majorAxisEndpoint.x() * ellipse.majorAxisEndpoint.x() +
                    ellipse.majorAxisEndpoint.y() * ellipse.majorAxisEndpoint.y());
                if (majorLen < 1e-10) continue;
                double minorLen = majorLen * ellipse.ratio;
                double angleRad = std::atan2(ellipse.majorAxisEndpoint.y(),
                    ellipse.majorAxisEndpoint.x());
                // 旋转到局部坐标系
                double cosA = std::cos(-angleRad), sinA = std::sin(-angleRad);
                double lx = dx * cosA - dy * sinA;
                double ly = dx * sinA + dy * cosA;
                // 椭圆方程: lx^2/a^2 + ly^2/b^2 = 1
                double val = (lx * lx) / (majorLen * majorLen) + (ly * ly) / (minorLen * minorLen);
                if (val < 1e-6) {
                    dist = 0;
                }
                else {
                    double sqrtVal = std::sqrt(val);
                    dist = std::abs(sqrtVal - 1.0) * std::min(majorLen, minorLen);
                }
                break;
            }
            case EntityType::LWPolyline:
            {
                const auto& poly = std::get<EntityLWPolyline>(entity);
                const auto& verts = poly.vecVertices;
                if (verts.empty()) continue;
                double minSegDist = std::numeric_limits<double>::max();
                int n = static_cast<int>(verts.size());
                for (int j = 0; j < n; ++j)
                {
                    int next = (j + 1) % n;
                    if (!poly.isClosed() && next == 0) break;
                    double d = pointToSegmentDist(scenePos,
                        QPointF(verts[j].point.x(), verts[j].point.y()),
                        QPointF(verts[next].point.x(), verts[next].point.y()));
                    minSegDist = std::min(minSegDist, d);
                }
                dist = minSegDist;
                break;
            }
            case EntityType::Polyline:
            {
                const auto& poly = std::get<EntityPolyline>(entity);
                const auto& verts = poly.vertices;
                if (verts.empty()) continue;
                double minSegDist = std::numeric_limits<double>::max();
                int n = static_cast<int>(verts.size());
                for (int j = 0; j < n; ++j)
                {
                    int next = (j + 1) % n;
                    if (!poly.isClosed() && next == 0) break;
                    double d = pointToSegmentDist(scenePos,
                        QPointF(verts[j].point.x(), verts[j].point.y()),
                        QPointF(verts[next].point.x(), verts[next].point.y()));
                    minSegDist = std::min(minSegDist, d);
                }
                dist = minSegDist;
                break;
            }
            case EntityType::Spline:
            {
                const auto& spline = std::get<EntitySpline>(entity);
                double minPtDist = std::numeric_limits<double>::max();
                // 用控制点做近似检测(足够精确)
                for (const auto& cp : spline.controlPoints)
                {
                    double d = QLineF(scenePos, QPointF(cp.x(), cp.y())).length();
                    minPtDist = std::min(minPtDist, d);
                }
                // 也检查拟合点(如果有)
                for (const auto& fp : spline.fitPoints)
                {
                    double d = QLineF(scenePos, QPointF(fp.x(), fp.y())).length();
                    minPtDist = std::min(minPtDist, d);
                }
                dist = minPtDist;
                break;
            }
            case EntityType::Text:
            {
                const auto& text = std::get<EntityText>(entity);
                // 文本近似包围盒检测
                double h = text.height;
                double w = h * 2.0;  // 估算宽度
                QPointF pt(text.insertPoint.x(), text.insertPoint.y());
                QRectF textRect(pt.x() - w * 0.1, pt.y() - h * 0.5, w, h);
                // 旋转处理
                if (std::abs(text.rotation) > 1e-6)
                {
                    QPointF center = textRect.center();
                    QPointF local = scenePos - center;
                    double rad = -text.rotation;
                    double cosR = std::cos(rad), sinR = std::sin(rad);
                    QPointF rotated(local.x() * cosR - local.y() * sinR,
                        local.x() * sinR + local.y() * cosR);
                    rotated += center;
                    dist = pointToRectDist(rotated, textRect);
                }
                else
                {
                    dist = pointToRectDist(scenePos, textRect);
                }
                if (dist < threshold * 2) dist *= 0.5;  // 文本更易选中
                break;
            }
            case EntityType::MText:
            {
                const auto& mtext = std::get<EntityMText>(entity);
                double h = mtext.height;
                double w = std::sqrt(mtext.xAxisDir.x() * mtext.xAxisDir.x() +
                    mtext.xAxisDir.y() * mtext.xAxisDir.y());
                if (w < 1.0) w = h * 10.0;
                QPointF pt(mtext.insertPoint.x(), mtext.insertPoint.y());
                QRectF mtextRect(pt.x(), pt.y() - h * 0.5, w, h);
                double angRad = std::atan2(mtext.xAxisDir.y(), mtext.xAxisDir.x());
                if (std::abs(angRad) > 1e-6)
                {
                    QPointF center = mtextRect.center();
                    QPointF local = scenePos - center;
                    double cosR = std::cos(-angRad), sinR = std::sin(-angRad);
                    QPointF rotated(local.x() * cosR - local.y() * sinR,
                        local.x() * sinR + local.y() * cosR);
                    rotated += center;
                    dist = pointToRectDist(rotated, mtextRect);
                }
                else
                {
                    dist = pointToRectDist(scenePos, mtextRect);
                }
                if (dist < threshold * 2) dist *= 0.5;
                break;
            }
            default:
                
                continue;
            }

            if (dist >= 0 && dist < minDist)
            {
                minDist = dist;
                hitLayer = QString::fromStdString(it->first);
                hitIndex = i;
            }
        }
    }

    if (hitIndex >= 0)
    {
        m_bEntitySelected = true;
        m_strSelectedLayer = hitLayer;
        m_nSelectedIndex = hitIndex;
        QRectF bounds = CalcEntityBounds(hitLayer, hitIndex);
        if (bounds.isValid())
        {
            m_pScene->ShowGrips(bounds);
        }
        emit signalEntitySelected(hitLayer, hitIndex);
    }
    else
    {
        ClearSelection();
    }
}

void CDxfTools::ClearSelection()
{
    if (m_bEntitySelected)
    {
        if (m_pScene)
            m_pScene->RemoveGrips();

        m_bEntitySelected = false;
        m_strSelectedLayer.clear();
        m_nSelectedIndex = -1;
        emit signalEntityDeselected();
    }
}


template<typename T>
static QRectF CalcVertsBoundsT(const std::vector<T>& verts, double s)
{
    if (verts.empty()) return QRectF();
    double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
    for (const auto& v : verts) {
        minX = std::min(minX, v.point.x());
        minY = std::min(minY, v.point.y());
        maxX = std::max(maxX, v.point.x());
        maxY = std::max(maxY, v.point.y());
    }
    return QRectF(minX - s, minY - s, maxX - minX + s * 2, maxY - minY + s * 2);
}

QRectF CDxfTools::CalcVertsBounds(const std::vector<PolylineVertex2D>& verts, double s)
{
    return CalcVertsBoundsT(verts, s);
}

QRectF CDxfTools::CalcVertsBounds(const std::vector<PolylineVertex3D>& verts, double s)
{
    return CalcVertsBoundsT(verts, s);
}

QRectF CDxfTools::CalcEntityBounds(const QString& strLayer, int entityIndex)
{
    if (!m_pData) return QRectF();

    const auto& layers = m_pData->GetLayers();
    auto it = layers.find(strLayer.toStdString());
    if (it == layers.end() || entityIndex < 0 ||
        entityIndex >= static_cast<int>(it->second.entities.size()))
        return QRectF();

    const auto& entity = it->second.entities[entityIndex];
    EntityType type = GetEntityType(entity);

    const double s = 5.0 / (m_pScene ? m_pScene->GetScale() : 1.0);

    switch (type)
    {
    case EntityType::Point:
    {
        const auto& pt = std::get<EntityPoint>(entity);
        return QRectF(pt.point.x() - s, pt.point.y() - s, s * 2, s * 2);
    }
    case EntityType::Line:
    {
        const auto& line = std::get<EntityLine>(entity);
        qreal x1 = line.startPoint.x(), y1 = line.startPoint.y();
        qreal x2 = line.endPoint.x(), y2 = line.endPoint.y();
        return QRectF(std::min(x1, x2), std::min(y1, y2),
            std::abs(x2 - x1), std::abs(y2 - y1)).adjusted(-s, -s, s, s);
    }
    case EntityType::Circle:
    {
        const auto& circle = std::get<EntityCircle>(entity);
        return QRectF(circle.center.x() - circle.radius - s,
            circle.center.y() - circle.radius - s,
            circle.radius * 2 + s * 2,
            circle.radius * 2 + s * 2);
    }
    case EntityType::Arc:
    {
        const auto& arc = std::get<EntityArc>(entity);
        return QRectF(arc.center.x() - arc.radius - s,
            arc.center.y() - arc.radius - s,
            arc.radius * 2 + s * 2,
            arc.radius * 2 + s * 2);
    }
    case EntityType::Ellipse:
    {
        const auto& ellipse = std::get<EntityEllipse>(entity);
        double majorLen = std::sqrt(
            ellipse.majorAxisEndpoint.x() * ellipse.majorAxisEndpoint.x() +
            ellipse.majorAxisEndpoint.y() * ellipse.majorAxisEndpoint.y());
        if (majorLen < 1e-10) return QRectF();
        double minorLen = majorLen * ellipse.ratio;
        double angleRad = std::atan2(ellipse.majorAxisEndpoint.y(),
            ellipse.majorAxisEndpoint.x());
        double cosA = std::cos(angleRad), sinA = std::sin(angleRad);
        double cx = ellipse.center.x(), cy = ellipse.center.y();
        // 采样 64 个点求精确包围盒
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        double startP = ellipse.startParam;
        double endP = ellipse.endParam;
        int numSamples = 64;
        for (int i = 0; i <= numSamples; ++i)
        {
            double theta = startP + (endP - startP) * i / numSamples;
            // 局部坐标 (长轴方向为 X)
            double lx = majorLen * std::cos(theta);
            double ly = minorLen * std::sin(theta);
            // 旋转到世界坐标
            double wx = cx + lx * cosA - ly * sinA;
            double wy = cy + lx * sinA + ly * cosA;
            minX = std::min(minX, wx);
            maxX = std::max(maxX, wx);
            minY = std::min(minY, wy);
            maxY = std::max(maxY, wy);
        }
        return QRectF(minX - s, minY - s,
            maxX - minX + s * 2, maxY - minY + s * 2);
    }
    case EntityType::LWPolyline:
    {
        const auto& poly = std::get<EntityLWPolyline>(entity);
        return CalcVertsBounds(poly.vecVertices, s);
    }
    case EntityType::Polyline:
    {
        const auto& poly = std::get<EntityPolyline>(entity);
        return CalcVertsBounds(poly.vertices, s);
    }
    case EntityType::Spline:
    {
        const auto& spline = std::get<EntitySpline>(entity);
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        auto update = [&](double x, double y) {
            minX = std::min(minX, x); minY = std::min(minY, y);
            maxX = std::max(maxX, x); maxY = std::max(maxY, y);
            };
        for (const auto& pt : spline.controlPoints)
            update(pt.x(), pt.y());
        for (const auto& pt : spline.fitPoints)
            update(pt.x(), pt.y());
        if (minX > maxX) return QRectF();
        return QRectF(minX - s, minY - s, maxX - minX + s * 2, maxY - minY + s * 2);
    }
    case EntityType::Text:
    {
        const auto& text = std::get<EntityText>(entity);
        double w = text.height * 3.0;   // 估算宽度
        double h = text.height;
        return QRectF(text.insertPoint.x() - s,
            text.insertPoint.y() - h * 0.5 - s,
            w + s * 2, h + s * 2);
    }
    case EntityType::MText:
    {
        const auto& mtext = std::get<EntityMText>(entity);
        double w = std::sqrt(mtext.xAxisDir.x() * mtext.xAxisDir.x() +
            mtext.xAxisDir.y() * mtext.xAxisDir.y());
        if (w < 1.0) w = mtext.height * 5.0;
        double h = mtext.height;
        return QRectF(mtext.insertPoint.x() - s,
            mtext.insertPoint.y() - s,
            w + s * 2, h + s * 2);
    }
    default:
        return QRectF();
    }
}
