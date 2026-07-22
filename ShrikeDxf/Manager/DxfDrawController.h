#pragma once
#include <QObject>
#include <QPointF>
#include <QVector>

#include "Tools.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"

class DxfDrawController : public QObject
{
    Q_OBJECT

public:
    DxfDrawController(DxfData* data, CDxfGraphicsScene* scene, QObject* parent = nullptr);
    ~DxfDrawController();

    // 获取当前图层
    const QString& getCurrentLayer() const;
    // 设置当前工具
    void setMouseStatus(MouseStateInView mouseState);
    // 鼠标移动
    void onMouseMove(QPointF scenePos);
    // 鼠标左键点击
    void onGraphicsViewLeftClick(QPointF scenePos);
    // 鼠标右键点击
    void onGraphicsViewRightClick(QPointF scenePos);

private:
    // 完成/闭合多段线
    void finishPolyline();
    // 取消当前多段线
    void cancelPolyline();
    // 完成矩形
    void finishRectangle(QPointF scenePos);
    // 完成椭圆
    void finishEllipse(QPointF scenePos);
    // 完成样条曲线(拟合点)
    void finishSplineFit();
    // 完成样条曲线(控制点)
    void finishSplineControl();
    // 取消当前样条
    void cancelSpline();
    // 完成文本
    void finishText(QPointF scenePos);
    // 完成多行文本
    void finishMText(QPointF scenePos);

private:

private:
    // 鼠标状态
    MouseStateInView m_currentState = MouseStateInView::enumMouseState_None;
    // 绘图的步骤
    int m_step = 0;

    QPointF m_startPoint;
    QPointF m_midPoint;
    QVector<QPointF> m_polyPoints;
    QVector<QPointF> m_splinePoints;

    DxfData* m_data = nullptr;
    CDxfGraphicsScene* m_scene = nullptr;
signals:
};
