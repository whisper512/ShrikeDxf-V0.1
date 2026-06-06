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
    if (m_eState != state)
    {
        // 从绘制状态退出 → 结束绘制预览
        if (IsDrawState(m_eState) && !IsDrawState(state))
        {
            //if (m_pDrawCtrl) m_pDrawCtrl->Cancel(); // 需要一个 Cancel 方法
        }
        // 从编辑状态退出 → 结束编辑操作
        if (IsEditState(m_eState) && !IsEditState(state))
        {
            //if (m_pEditCtrl) m_pEditCtrl->FinishOperation();
        }
    }

    m_eState = state;

    // 进入状态时的初始化
    if (IsDrawState(state) && m_pDrawCtrl)
    {
        m_pDrawCtrl->SetMouseStatus(state);
    }
    else if (IsEditState(state) && m_pEditCtrl)
    {
        //m_pEditCtrl->BeginOperation(state);  // 编辑控制器开始操作
    }
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

bool CDxfInteractionDispatcher::IsDrawState(enumMouseStateInView state) const
{
    return state >= enumMouseStateInView::enumMouseState_Point &&
        state <= enumMouseStateInView::enumMouseState_MText;
}

bool CDxfInteractionDispatcher::IsEditState(enumMouseStateInView state) const
{
    return state >= enumMouseStateInView::enumMouseState_Move;  // 或更精细的判断
}

