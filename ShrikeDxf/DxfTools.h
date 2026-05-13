#pragma once
#include <QObject>
#include <QPointF>
#include <QVector>
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
    // 鼠标右键点击
    void OnGraphicsViewRightClick(QPointF scenePos);
    // 完成/闭合多段线
    void FinishPolyline();
    // 取消当前多段线
    void CancelPolyline();
    // 完成矩形
    void FinishRectangle(QPointF scenePos);
    // 完成椭圆
    void FinishEllipse(QPointF scenePos);

private:
    enumMouseStateInView m_eCurrentTool = enumMouseStateInView::enumMouseState_None;
    int m_step = 0;

    QPointF m_ptStart;
    QPointF m_ptMid;
    QVector<QPointF> m_vecPolyPoints;

    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;

};
