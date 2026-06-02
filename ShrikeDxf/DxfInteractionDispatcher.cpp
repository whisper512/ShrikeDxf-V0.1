#include "DxfInteractionDispatcher.h"
#include "DxfDrawController.h"
#include "DxfEditController.h"
#include "DxfSelectionController.h"


CDxfInteractionDispatcher::CDxfInteractionDispatcher(QObject* parent)
    : QObject(parent)
{
}

void CDxfInteractionDispatcher::SetControllers(CDxfDrawController* drawCtrl, CDxfEditController* editCtrl, CSelectionController* selCtrl)
{
    m_pDrawCtrl = drawCtrl;
    m_pEditCtrl = editCtrl;
    m_pSelectionCtrl = selCtrl;
}

void CDxfInteractionDispatcher::SetMouseStatus(enumMouseStateInView state)
{
    if (state == enumMouseStateInView::enumMouseState_None)
    {
        m_pSelectionCtrl->ClearSelection();
    }
    else
    {
        if (m_pDrawCtrl)
            m_pDrawCtrl->SetMouseStatus(state);
    }
    m_eState = state;
}

void CDxfInteractionDispatcher::OnMouseMove(QPointF scenePos)
{
    if (m_eState == enumMouseStateInView::enumMouseState_None)
    {

    }
    else
    {
        // 绘制模式交给绘制控制器
        m_pDrawCtrl->OnMouseMove(scenePos);
    }
}

void CDxfInteractionDispatcher::OnLeftClick(QPointF scenePos)
{
    if (!m_pDrawCtrl)
        return;

    if (m_eState == enumMouseStateInView::enumMouseState_None)
    {
        // 光标模式下左键交给选择控制器
        if (m_pSelectionCtrl)
            m_pSelectionCtrl->HitTest(scenePos);
    }
    else
    {
        // 绘制模式交给绘制控制器
        m_pDrawCtrl->OnGraphicsViewLeftClick(scenePos);
    }
}

void CDxfInteractionDispatcher::OnRightClick(QPointF scenePos)
{
    if (m_pDrawCtrl)
        m_pDrawCtrl->OnGraphicsViewRightClick(scenePos);

    // 如果以后编辑模式需要右键菜单，可在此扩展
}
