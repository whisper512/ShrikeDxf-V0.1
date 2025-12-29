#include "GraphicsView.h"
#include "ShrikeDxf.h"

CGraphicsView::CGraphicsView()
{
}

CGraphicsView::~CGraphicsView()
{
}

CGraphicsView::CGraphicsView(QWidget* pMainwnd):
    m_pMainWnd(pMainwnd)
{
    ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
    if (pWnd)
    {
        pWnd->ui.verticalLayout_Main->addWidget(this);

        setAutoFillBackground(true);

        setStyleSheet("background-color:#d0d0d0;");

        //翻转y轴
        setTransform(QTransform(1, 0, 0, -1, 0, 0));

        // 确保视图可见
        setVisible(true);
        show();
    }
}

void CGraphicsView::wheelEvent(QWheelEvent* pEvent)
{
    // 获取鼠标位置在场景中的坐标
    QPointF mouseScenePos = mapToScene(pEvent->position().toPoint());
    // 缩放因子
    const double scaleFactor = 1.15;
    double scaleFactorActual = (pEvent->angleDelta().y() > 0) ? scaleFactor : 1.0 / scaleFactor;
    // 获取当前视图中心点在场景中的坐标
    QPointF viewCenterScene = mapToScene(viewport()->rect().center());
    // 计算新的视图中心点
    QPointF newCenter = mouseScenePos + (viewCenterScene - mouseScenePos) / scaleFactorActual;
    // 执行缩放
    scale(scaleFactorActual, scaleFactorActual);
    // 设置新的视图中心
    centerOn(newCenter);
}



void CGraphicsView::handleRefreshGraphicsview(CDxfGraphicsScene* pScene)
{
    if (pScene)
    {
        // 设置场景到视图
        setScene(pScene);

        // 计算所有项目的边界矩形
        QRectF boundingRect;
        for (const auto& item : pScene->items()) 
        {
            boundingRect = boundingRect.united(item->boundingRect());
        }
        QTimer::singleShot(0, [this, pScene]() {fitInView(pScene->sceneRect(), Qt::KeepAspectRatio);});
        // 强制更新视图
        update();

    }
    return;

}