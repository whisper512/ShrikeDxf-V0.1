#pragma once
#include <QObject>
#include <QPointF>
#include <QVector>
#include "Tools.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"

class CDxfEditController;

class CDxfDrawController : public QObject
{
    Q_OBJECT

public:
    CDxfDrawController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CDxfDrawController();

    const QString& GetCurrentLayer() const;
    // 设置编辑控制类
    void SetEditController(CDxfEditController* pEdit) { m_pEditController = pEdit; }
    // 设置当前工具
    void SetMouseStatus(enumMouseStateInView mouseState);
    // 鼠标移动
    void OnMouseMove(QPointF scenePos);
    // 鼠标左键点击
    void OnGraphicsViewLeftClick(QPointF scenePos);
    // 鼠标右键点击
    void OnGraphicsViewRightClick(QPointF scenePos);
    // 鼠标左键按下(用于拖拽)
    void OnGraphicsViewLeftPress(QPointF scenePos);
    // 鼠标左键释放(用于拖拽)
    void OnGraphicsViewLeftRelease(QPointF scenePos);
    // 开始拖拽手柄
    void OnGripDragStarted(int gripIndex);
    // 拖拽手柄
    void OnGripDragged(int gripIndex, QPointF newPos);
    // 拖拽手柄结束
    void OnGripDragFinished(int gripIndex, QPointF finalPos);

private:
    CDxfEditController* m_pEditController = nullptr;
    // 完成/闭合多段线
    void FinishPolyline();
    // 取消当前多段线
    void CancelPolyline();
    // 完成矩形
    void FinishRectangle(QPointF scenePos);
    // 完成椭圆
    void FinishEllipse(QPointF scenePos);
    // 完成样条曲线(拟合点)
    void FinishSplineFit();
    // 完成样条曲线(控制点)
    void FinishSplineControl();
    // 取消当前样条
    void CancelSpline();
    // 完成文本
    void FinishText(QPointF scenePos);
    // 完成多行文本
    void FinishMText(QPointF scenePos);

private:



private:
    // 鼠标状态
    enumMouseStateInView m_eCurrentTool = enumMouseStateInView::enumMouseState_None;
    // 绘图的步骤
    int m_step = 0;

    QPointF m_ptStart;
    QPointF m_ptMid;
    QVector<QPointF> m_vecPolyPoints;
    QVector<QPointF> m_vecSplinePoints;

    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;

    // 选中图元手柄索引
    int m_nDragGripIndex = -1;


signals:
    // 绘制图元的状态
    void signalDrawingModeChanged(bool active);


};
