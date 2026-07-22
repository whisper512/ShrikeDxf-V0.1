#pragma once
#include <QObject>
#include <QPointF>
#include "Tools.h"

class dxfDrawController;
class DxfEditController;
class CSelectionController;

// graphicsView交互控制类
class CDxfInteractionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit CDxfInteractionDispatcher(QObject* parent = nullptr);
    ~CDxfInteractionDispatcher();

    // 持有其他数据类
    void SetControllers(dxfDrawController* drawCtrl, DxfEditController* editCtrl ,CSelectionController* selCtrl);
    // 设置当前工具,修改鼠标状态
    void SetMouseStatus(MouseStateInView state);
    // 发会鼠标状态
    MouseStateInView GetCurrentState() const { return m_eState; }
    // 鼠标事件
    void OnMouseMove(QPointF scenePos);
    void OnLeftClick(QPointF scenePos);
    void OnRightClick(QPointF scenePos);
    void OnLeftPress(QPointF scenePos);
    void OnLeftRelease(QPointF scenePos);


private:
    // 当前鼠标状态
    MouseStateInView m_eState = MouseStateInView::enumMouseState_None;
    // 绘制控制器
    dxfDrawController* m_pDrawCtrl = nullptr;
    // 编辑控制器
    DxfEditController* m_pEditCtrl = nullptr;
    // 选择控制器
    CSelectionController* m_selectionCtrl = nullptr;

private:
    bool IsDrawState(MouseStateInView state) const;
    bool IsEditState(MouseStateInView state) const;
};
