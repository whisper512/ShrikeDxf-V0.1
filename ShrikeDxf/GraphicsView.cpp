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

        setStyleSheet("background-color:#f0f0f0;");

        // 确保视图可见
        setVisible(true);
        show();
    }
}



void CGraphicsView::handleRefreshGraphicsview(CDxfGraphicsScene* pScene)
{
    if (pScene)
    {
        // 设置场景到视图
        setScene(pScene);

        // 添加一些测试图形
        // 1. 添加一个矩形
        pScene->addRect(50, 50, 200, 150, QPen(Qt::black, 2), QBrush(Qt::yellow));

        // 2. 添加一个圆形
        pScene->addEllipse(300, 50, 100, 100, QPen(Qt::red, 2), QBrush(Qt::transparent));

        // 3. 添加一条线
        pScene->addLine(50, 250, 350, 250, QPen(Qt::blue, 3));


        // 5. 添加一个三角形
        QPolygonF triangle;
        triangle << QPointF(200, 350) << QPointF(150, 400) << QPointF(250, 400);
        pScene->addPolygon(triangle, QPen(Qt::darkGreen, 2), QBrush(Qt::green));

        // 调整视图以显示所有内容
        fitInView(pScene->sceneRect(), Qt::KeepAspectRatio);

        // 强制更新视图
        update();

    }
    return;

}