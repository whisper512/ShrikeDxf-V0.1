#pragma once
#include <QObject>
#include <QPointF>
#include "Tools.h"

class CDxfDrawController;
class CDxfEditController;
class CSelectionController;

// graphicsView交互控制类
class CDxfInteractionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit CDxfInteractionDispatcher(QObject* parent = nullptr);
    ~CDxfInteractionDispatcher();

    // 持有其他数据类
    void SetControllers(CDxfDrawController* drawCtrl, CDxfEditController* editCtrl ,CSelectionController* selCtrl);
    // 设置当前工具,修改鼠标状态
    void SetMouseStatus(enumMouseStateInView state);
    // 发会鼠标状态
    enumMouseStateInView GetCurrentState() const { return m_eState; }
    // 鼠标事件
    void OnMouseMove(QPointF scenePos);
    void OnLeftClick(QPointF scenePos);
    void OnRightClick(QPointF scenePos);

private:
    enumMouseStateInView m_eState = enumMouseStateInView::enumMouseState_None;
    CDxfDrawController* m_pDrawCtrl = nullptr;
    CDxfEditController* m_pEditCtrl = nullptr;
    CSelectionController* m_pSelectionCtrl = nullptr;

private:
    bool IsDrawState(enumMouseStateInView state) const;
    bool IsEditState(enumMouseStateInView state) const;
};
