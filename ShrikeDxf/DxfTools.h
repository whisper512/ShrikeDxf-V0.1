#pragma once
#include <QObject>
#include <QPointF>
#include "Tools.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"

class CDxfTools : public QObject
{
    Q_OBJECT

public:
    CDxfTools(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CDxfTools();

    const QString& GetCurrentLayer() const;

    // 设置当前工具
    void SetMouseStatus(enumMouseStateInView mouseState);
    // 鼠标移动
    void OnMouseMove(QPointF scenePos);
    // 鼠标左键点击
    void OnGraphicsViewLeftClick(QPointF scenePos);

private:
    enumMouseStateInView m_eCurrentTool = enumMouseStateInView::enumMouseState_None;
    int m_step = 0;

    QPointF m_ptStart;

    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;
};
