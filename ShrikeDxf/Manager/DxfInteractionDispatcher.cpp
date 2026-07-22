#include "DxfInteractionDispatcher.h"
#include "dxfDrawController.h"
#include "DxfEditController.h"
#include "DxfSelectionController.h"


CDxfInteractionDispatcher::CDxfInteractionDispatcher(QObject* parent)
    : QObject(parent)
{
}

CDxfInteractionDispatcher::~CDxfInteractionDispatcher()
{
    
}

void CDxfInteractionDispatcher::SetControllers(dxfDrawController* drawCtrl, DxfEditController* editCtrl, CSelectionController* selCtrl)
{
    m_pDrawCtrl = drawCtrl;
    m_pEditCtrl = editCtrl;
    m_selectionCtrl = selCtrl;
}

void CDxfInteractionDispatcher::SetMouseStatus(MouseStateInView state)
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
        m_pDrawCtrl->setMouseStatus(state);
    }
    else if (IsEditState(state) && m_pEditCtrl)
    {
        //m_pEditCtrl->BeginOperation(state);  // 编辑控制器开始操作
    }
}

void CDxfInteractionDispatcher::OnMouseMove(QPointF scenePos)
{
    // 拉伸模式优先
    if (m_pEditCtrl && m_pEditCtrl->isStretching())
    {
        //m_pEditCtrl->updateStretch(scenePos);
        return;
    }

    if (m_eState == MouseStateInView::enumMouseState_None)
    {
        // 可以在这里加 hover 夹点高亮
    }
    else
    {
        m_pDrawCtrl->onMouseMove(scenePos);
    }
}


void CDxfInteractionDispatcher::OnLeftClick(QPointF scenePos)
{
    if (!m_pDrawCtrl)
        return;

    if (m_eState == MouseStateInView::enumMouseState_None)
    {
        // 先检查是否命中了选中图元的夹点
        if (m_pEditCtrl)
        {
            StretchGripInView grip = m_pEditCtrl->hitTestGrip(scenePos);
            if (grip != StretchGripInView::None)
                return;
        }

        // 没有命中夹点才进行选择
        if (m_selectionCtrl)
            m_selectionCtrl->HitTest(scenePos);
    }
    else
    {
        m_pDrawCtrl->onGraphicsViewLeftClick(scenePos);
    }
}


void CDxfInteractionDispatcher::OnRightClick(QPointF scenePos)
{
    if (m_pDrawCtrl)
        m_pDrawCtrl->onGraphicsViewRightClick(scenePos);

    // 如果以后编辑模式需要右键菜单，可在此扩展
}

void CDxfInteractionDispatcher::OnLeftPress(QPointF scenePos)
{
    // 仅在光标模式(非工具模式)且编辑控制器存在时检测夹点
    if (m_eState != MouseStateInView::enumMouseState_None)
        return;
    if (!m_pEditCtrl)
        return;

    StretchGripInView grip = m_pEditCtrl->hitTestGrip(scenePos);
    if (grip != StretchGripInView::None)
    {
        m_pEditCtrl->startStretch(grip);
    }
}


void CDxfInteractionDispatcher::OnLeftRelease(QPointF scenePos)
{
    Q_UNUSED(scenePos);

    if (m_pEditCtrl && m_pEditCtrl->isStretching())
    {
        m_pEditCtrl->endStretch(scenePos);
    }
}




bool CDxfInteractionDispatcher::IsDrawState(MouseStateInView state) const
{
    return state >= MouseStateInView::enumMouseState_Point &&
        state <= MouseStateInView::enumMouseState_MText;
}

bool CDxfInteractionDispatcher::IsEditState(MouseStateInView state) const
{
    // 或更精细的判断
    return state >= MouseStateInView::enumMouseState_Move;  
}

