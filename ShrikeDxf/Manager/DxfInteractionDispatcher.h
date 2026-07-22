#pragma once
#include <QObject>
#include <QPointF>
#include "Tools.h"

class dxfDrawController;
class DxfEditController;
class SelectionController;

// graphicsView交互控制类
class DxfInteractionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit DxfInteractionDispatcher(QObject* parent = nullptr);
    ~DxfInteractionDispatcher();

    // 持有其他数据类
    void setControllers(dxfDrawController* drawCtrl, DxfEditController* editCtrl ,SelectionController* selCtrl);
    // 设置当前工具,修改鼠标状态
    void setMouseStatus(MouseStateInView state);
    // 发会鼠标状态
    MouseStateInView getCurrentState() const { return m_state; }
    // 鼠标事件
    void onMouseMove(QPointF scenePos);
    void onLeftClick(QPointF scenePos);
    void onRightClick(QPointF scenePos);
    void onLeftPress(QPointF scenePos);
    void onLeftRelease(QPointF scenePos);


private:
    // 当前鼠标状态
    MouseStateInView m_state = MouseStateInView::enumMouseState_None;
    // 绘制控制器
    dxfDrawController* m_drawCtrl = nullptr;
    // 编辑控制器
    DxfEditController* m_editCtrl = nullptr;
    // 选择控制器
    SelectionController* m_selectionCtrl = nullptr;

private:
    bool isDrawState(MouseStateInView state) const;
    bool isEditState(MouseStateInView state) const;
};
