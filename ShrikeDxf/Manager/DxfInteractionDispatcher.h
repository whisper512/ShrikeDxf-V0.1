#pragma once
#include <QObject>
#include <QPointF>
#include "Tools.h"

class CDxfDrawController;
class CDxfEditController;
class CSelectionController;

// 交互控制类
class CDxfInteractionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit CDxfInteractionDispatcher(QObject* parent = nullptr);

    void SetControllers(CDxfDrawController* drawCtrl, CDxfEditController* editCtrl ,CSelectionController* selCtrl);
    void SetSelectionController(CSelectionController* pSelCtrl) { m_pSelectionCtrl = pSelCtrl; }

    // 设置当前工具
    void SetMouseStatus(enumMouseStateInView state);

    // 鼠标事件转发
    void OnMouseMove(QPointF scenePos);
    void OnLeftClick(QPointF scenePos);
    void OnRightClick(QPointF scenePos);

    enumMouseStateInView GetCurrentState() const { return m_eState; }

private:
    enumMouseStateInView m_eState = enumMouseStateInView::enumMouseState_None;
    CDxfDrawController* m_pDrawCtrl = nullptr;
    CDxfEditController* m_pEditCtrl = nullptr;
    CSelectionController* m_pSelectionCtrl = nullptr;

private:
    bool IsDrawState(enumMouseStateInView state) const;
    bool IsEditState(enumMouseStateInView state) const;
};
