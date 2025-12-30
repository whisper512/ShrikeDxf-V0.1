#include "GraphicsView.h"
#include "StyleSheet.h"
#include "ShrikeDxf.h"

CGraphicsView::CGraphicsView()
{
}

CGraphicsView::~CGraphicsView()
{
}

CGraphicsView::CGraphicsView(QWidget* pMainwnd):
    m_pMainWnd(pMainwnd),
    m_pGraphicsViewMenu(nullptr),
    m_pActionLockZoom(nullptr),
    m_pActionFilpX(nullptr),
    m_pActionFilpY(nullptr),
    m_bLockZoom(false),
    m_bFilpAlongX(false),
    m_bFilpAlongY(false)
{
    //添加graphicsview到layout
    ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
    if (pWnd)
    {
        pWnd->ui.verticalLayout_Main->addWidget(this);
        setAutoFillBackground(true);
        setStyleSheet(BACKGROUND_GRAY);
        //翻转y轴
        setTransform(QTransform(1, 0, 0, -1, 0, 0));
        // 确保视图可见
        setVisible(true);
        show();
    }
    InitMenu(this);
}

void CGraphicsView::InitMenu(QWidget* pParent)
{
    m_pGraphicsViewMenu = new QMenu();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CGraphicsView::ShowMenu);

    InitAction();

}

void CGraphicsView::ShowMenu(const QPoint& pos)
{
    m_pGraphicsViewMenu->popup(mapToGlobal(pos));
}


void CGraphicsView::InitAction()
{
    m_pActionLockZoom = new QAction("Lock Zoom", this);
    m_pActionLockZoom->setCheckable(true);
    m_pActionLockZoom->setChecked(false);
    m_pActionFilpX = new QAction("Horizontal Flip", this);
    m_pActionFilpX->setCheckable(true);
    m_pActionFilpX->setChecked(false);
    m_pActionFilpY = new QAction("Vertical Flip", this);
    m_pActionFilpY->setCheckable(true);
    m_pActionFilpY->setChecked(false);

    m_pGraphicsViewMenu->addAction(m_pActionLockZoom);
    m_pGraphicsViewMenu->addAction(m_pActionFilpX);
    m_pGraphicsViewMenu->addAction(m_pActionFilpY);

    connect(m_pActionLockZoom, &QAction::toggled, this, &CGraphicsView::handleLockZoom);
    connect(m_pActionFilpX, &QAction::toggled, this, &CGraphicsView::handleFilpAlongX);
    connect(m_pActionFilpY, &QAction::toggled, this, &CGraphicsView::handleFilpAlongY);
}

void CGraphicsView::handleFilpAlongX(bool bChecked)
{
    m_bFilpAlongX = bChecked;
    QTransform transform;
    if (m_bFilpAlongX && m_bFilpAlongY)
    {
        // x轴和y轴都翻转
        transform = QTransform(-1, 0, 0, 1, 0, 0);
    }
    else if (m_bFilpAlongX)
    {
        // 只翻转x轴
        transform = QTransform(-1, 0, 0, -1, 0, 0);
    }
    else if (m_bFilpAlongY)
    {
        // 只翻转y轴（双重翻转，相当于恢复到正常方向）
        transform = QTransform(1, 0, 0, 1, 0, 0);
    }
    else
    {
        // 恢复到初始状态（y轴翻转）
        transform = QTransform(1, 0, 0, -1, 0, 0);
    }
    setTransform(transform);
}

void CGraphicsView::handleFilpAlongY(bool bChecked)
{
    m_bFilpAlongY = bChecked;
    QTransform transform;
    if (m_bFilpAlongX && m_bFilpAlongY)
    {
        // x轴和y轴都翻转
        transform = QTransform(-1, 0, 0, 1, 0, 0);
    }
    else if (m_bFilpAlongX)
    {
        // 只翻转x轴
        transform = QTransform(-1, 0, 0, -1, 0, 0);
    }
    else if (m_bFilpAlongY)
    {
        // 只翻转y轴（双重翻转，相当于恢复到正常方向）
        transform = QTransform(1, 0, 0, 1, 0, 0);
    }
    else
    {
        // 恢复到初始状态（y轴翻转）
        transform = QTransform(1, 0, 0, -1, 0, 0);
    }
    setTransform(transform);
}

void CGraphicsView::handleLockZoom(bool bChecked)
{
    m_bLockZoom = bChecked;
}

void CGraphicsView::wheelEvent(QWheelEvent* pEvent)
{
    if (m_bLockZoom)
    {
        return;
    }
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