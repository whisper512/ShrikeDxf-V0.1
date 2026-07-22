#include "DxfInteractionDispatcher.h"
#include "dxfDrawController.h"
#include "DxfEditController.h"
#include "DxfSelectionController.h"


DxfInteractionDispatcher::DxfInteractionDispatcher(QObject* parent)
    : QObject(parent)
{
}

DxfInteractionDispatcher::~DxfInteractionDispatcher()
{
    
}

void DxfInteractionDispatcher::setControllers(dxfDrawController* drawCtrl, DxfEditController* editCtrl, SelectionController* selCtrl)
{
    m_drawCtrl = drawCtrl;
    m_editCtrl = editCtrl;
    m_selectionCtrl = selCtrl;
}

void DxfInteractionDispatcher::setMouseStatus(MouseStateInView state)
{
    if (m_state != state)
    {
        // 从绘制状态退出 → 结束绘制预览
        if (isDrawState(m_state) && !isDrawState(state))
        {
            //if (m_drawCtrl) m_drawCtrl->Cancel(); // 需要一个 Cancel 方法
        }
        // 从编辑状态退出 → 结束编辑操作
        if (isEditState(m_state) && !isEditState(state))
        {
            //if (m_editCtrl) m_editCtrl->FinishOperation();
        }
    }

    m_state = state;

    // 进入状态时的初始化
    if (isDrawState(state) && m_drawCtrl)
    {
        m_drawCtrl->setMouseStatus(state);
    }
    else if (isEditState(state) && m_editCtrl)
    {
        //m_editCtrl->BeginOperation(state);  // 编辑控制器开始操作
    }
}

void DxfInteractionDispatcher::onMouseMove(QPointF scenePos)
{
    // 拉伸模式优先
    if (m_editCtrl && m_editCtrl->isStretching())
    {
        //m_editCtrl->updateStretch(scenePos);
        return;
    }

    if (m_state == MouseStateInView::enumMouseState_None)
    {
        // 可以在这里加 hover 夹点高亮
    }
    else
    {
        m_drawCtrl->onMouseMove(scenePos);
    }
}


void DxfInteractionDispatcher::onLeftClick(QPointF scenePos)
{
    if (!m_drawCtrl)
        return;

    if (m_state == MouseStateInView::enumMouseState_None)
    {
        // 先检查是否命中了选中图元的夹点
        if (m_editCtrl)
        {
            StretchGripInView grip = m_editCtrl->hitTestGrip(scenePos);
            if (grip != StretchGripInView::None)
                return;
        }

        // 没有命中夹点才进行选择
        if (m_selectionCtrl)
            m_selectionCtrl->hitTest(scenePos);
    }
    else
    {
        m_drawCtrl->onGraphicsViewLeftClick(scenePos);
    }
}


void DxfInteractionDispatcher::onRightClick(QPointF scenePos)
{
    if (m_drawCtrl)
        m_drawCtrl->onGraphicsViewRightClick(scenePos);

    // 如果以后编辑模式需要右键菜单，可在此扩展
}

void DxfInteractionDispatcher::onLeftPress(QPointF scenePos)
{
    // 仅在光标模式(非工具模式)且编辑控制器存在时检测夹点
    if (m_state != MouseStateInView::enumMouseState_None)
        return;
    if (!m_editCtrl)
        return;

    StretchGripInView grip = m_editCtrl->hitTestGrip(scenePos);
    if (grip != StretchGripInView::None)
    {
        m_editCtrl->startStretch(grip);
    }
}


void DxfInteractionDispatcher::onLeftRelease(QPointF scenePos)
{
    Q_UNUSED(scenePos);

    if (m_editCtrl && m_editCtrl->isStretching())
    {
        m_editCtrl->endStretch(scenePos);
    }
}




bool DxfInteractionDispatcher::isDrawState(MouseStateInView state) const
{
    return state >= MouseStateInView::enumMouseState_Point &&
        state <= MouseStateInView::enumMouseState_MText;
}

bool DxfInteractionDispatcher::isEditState(MouseStateInView state) const
{
    // 或更精细的判断
    return state >= MouseStateInView::enumMouseState_Move;  
}

