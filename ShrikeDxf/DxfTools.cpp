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

void CDxfTools::SetMousePos(QPointF scenePos)
{
    if (!m_pScene) return;
    if (m_eCurrentTool == enumMouseStateInView::enumMouseState_Point)
    {
        m_pScene->ClearPreview();
        m_pScene->AddPreviewPoint(scenePos);
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
    default:
        break;
    }
}
