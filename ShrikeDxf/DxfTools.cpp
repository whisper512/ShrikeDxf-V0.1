#include "DxfTools.h"
#include "DxfManager.h"          // <--- 新增头文件，才能访问 CDxfManager

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
}

void CDxfTools::OnMouseMove(QPointF scenePos)
{
    if (!m_pScene) return;

    if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Point)
    {
        m_pScene->ClearPreview();
        m_pScene->AddPreviewPoint(scenePos);
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Line)
    {
        if (m_step == 1)
        {
            // 已选起点，画从起点到鼠标位置的预览线
            m_pScene->ClearPreview();
            m_pScene->AddPreviewLine(m_ptStart, scenePos);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_CircleCenterRadius)
    {
        if (m_step == 1)
        {
            qreal radius = QLineF(m_ptStart, scenePos).length();
            m_pScene->ClearPreview();
            m_pScene->AddPreviewCircle(m_ptStart, radius);
        }
    }
    else if (m_eCurrentTool == enumMouseStateInView::enumMouseState_CircleDiameter)
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
}

void CDxfTools::OnGraphicsViewLeftClick(QPointF scenePos)
{
    if (!m_pData || !m_pScene) return;

    switch (m_eCurrentTool)
    {
    case enumMouseStateInView::enumMouseState_None:
        break;

    case enumMouseStateInView::enumMouseState_Point:
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
    case enumMouseStateInView::enumMouseState_Line:
    {
        if (m_step == 0)
        {
            // 第一步:记录起点
            m_ptStart = scenePos;
            m_step = 1;
            // 在起点处画一个预览点，提示已选起点
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步:终点点击，创建实际线段
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
            // 清理预览，重绘场景
            m_pScene->ClearPreview();
            m_pScene->DxfDraw(m_pData->GetLayers());
            // 重置步骤
            m_step = 0;
        }
        break;
    }
    // 半径画圆
    case enumMouseStateInView::enumMouseState_CircleCenterRadius:
    {
        if (m_step == 0)
        {
            // 第一步：记录圆心
            m_ptStart = scenePos;
            m_step = 1;
            // 在圆心处画预览十字
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步：点击确定半径，创建实际圆
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
    // 直径画圆 
    case enumMouseStateInView::enumMouseState_CircleDiameter:
    {
        if (m_step == 0)
        {
            // 第一步：记录直径起点
            m_ptStart = scenePos;
            m_step = 1;
            m_pScene->ClearPreview();
            m_pScene->AddPreviewPoint(m_ptStart);
        }
        else if (m_step == 1)
        {
            // 第二步：点击确定直径终点
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
    default:
        break;
    }
}
