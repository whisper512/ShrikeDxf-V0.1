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
    m_pLabelMousePos(nullptr),
    m_pActionLockZoom(nullptr),
    m_pActionFilpX(nullptr),
    m_pActionFilpY(nullptr),
    m_pActionResetView(nullptr),
    m_pActionShowMousePos(nullptr),
    m_pActionDrag(nullptr),
    m_bLockZoom(false),
    m_bFilpAlongX(false),
    m_bFilpAlongY(false),
    m_bShowMousePos(false),
    m_bDrag(false),
    m_tranformInitial(1, 0, 0, -1, 0, 0),
    m_rectInitialScene(0, 0, 0, 0),
    m_pointLastPos(0, 0),
    m_rectLastScene(0, 0, 0, 0)
{
    //添加graphicsview到layout
    ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
    if (pWnd)
    {
        pWnd->ui.verticalLayout_Main->addWidget(this);
        setAutoFillBackground(true);
        setStyleSheet(BACKGROUND_GRAY);
        //翻转y轴
        setTransform(m_tranformInitial);
        // 确保视图可见
        setVisible(true);
        show();
    }
    //开启鼠标追踪
    setMouseTracking(true);
    InitMenu(this);
    InitPosLabel();
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
    m_pActionShowMousePos = new QAction("Show Pos", this);
    m_pActionShowMousePos->setCheckable(true);
    m_pActionShowMousePos->setChecked(false);
    m_pActionDrag = new QAction("Drag", this);
    m_pActionDrag->setCheckable(true);
    m_pActionDrag->setChecked(false);
    m_pActionLockZoom = new QAction("Lock Zoom", this);
    m_pActionLockZoom->setCheckable(true);
    m_pActionLockZoom->setChecked(false);
    m_pActionFilpX = new QAction("Horizontal Flip", this);
    m_pActionFilpX->setCheckable(true);
    m_pActionFilpX->setChecked(false);
    m_pActionFilpY = new QAction("Vertical Flip", this);
    m_pActionFilpY->setCheckable(true);
    m_pActionFilpY->setChecked(false);


    m_pActionResetView = new QAction("Reset View", this);

    m_pGraphicsViewMenu->addAction(m_pActionShowMousePos);
    m_pGraphicsViewMenu->addAction(m_pActionDrag);
    m_pGraphicsViewMenu->addAction(m_pActionLockZoom);
    m_pGraphicsViewMenu->addAction(m_pActionFilpX);
    m_pGraphicsViewMenu->addAction(m_pActionFilpY);
    m_pGraphicsViewMenu->addAction(m_pActionResetView);

    connect(m_pActionShowMousePos, &QAction::toggled, this, &CGraphicsView::handleShowMousePos);
    connect(m_pActionDrag, &QAction::toggled, this, &CGraphicsView::handleDrag);
    connect(m_pActionLockZoom, &QAction::toggled, this, &CGraphicsView::handleLockZoom);
    connect(m_pActionFilpX, &QAction::toggled, this, &CGraphicsView::handleFilpAlongX);
    connect(m_pActionFilpY, &QAction::toggled, this, &CGraphicsView::handleFilpAlongY);
    connect(m_pActionResetView, &QAction::triggered, this, &CGraphicsView::handleResetView);
}

void CGraphicsView::InitPosLabel()
{
    m_pLabelMousePos = new QLabel(this);
    m_pLabelMousePos->setStyleSheet("background-color: rgba(255, 255, 255, 200); padding: 2px;border-radius: 4px;");
    m_pLabelMousePos->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_pLabelMousePos->setText("X:0.0 Y:0.0");
    m_pLabelMousePos->adjustSize();
    m_pLabelMousePos->clear();
    m_pLabelMousePos->hide();
}

void CGraphicsView::FilpView()
{
    QTransform curTransform = transform();
    // 当前缩放比例,只取绝对值
    double curScale = std::abs(curTransform.m11());

    QTransform transform;
    if (m_bFilpAlongX && m_bFilpAlongY)
    {
        // x轴和y轴都翻转
        transform = QTransform(-curScale, 0, 0, curScale, 0, 0);
    }
    else if (m_bFilpAlongX)
    {
        // 只翻转x轴
        transform = QTransform(-curScale, 0, 0, -curScale, 0, 0);
    }
    else if (m_bFilpAlongY)
    {
        // 只翻转y轴（双重翻转，相当于恢复到正常方向）
        transform = QTransform(curScale, 0, 0, curScale, 0, 0);
    }
    else
    {
        // 恢复到初始状态（y轴翻转）
        transform = QTransform(curScale, 0, 0, -curScale, 0, 0);
    }
    setTransform(transform);
}

void CGraphicsView::handleFilpAlongX(bool bChecked)
{
    m_bFilpAlongX = bChecked;
    FilpView();
}

void CGraphicsView::handleFilpAlongY(bool bChecked)
{
    m_bFilpAlongY = bChecked;
    FilpView();
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

void CGraphicsView::handleResetView()
{
    setTransform(m_tranformInitial);
    if (scene())
    {
        fitInView(m_rectInitialScene, Qt::KeepAspectRatio);
    }
}

void CGraphicsView::handleShowMousePos(bool bChecked)
{
    m_bShowMousePos = bChecked;
    if (m_pLabelMousePos)
    {
        m_pLabelMousePos->setVisible(bChecked);
    }
}

void CGraphicsView::handleDrag(bool bChecked)
{
    m_bDrag = bChecked;
}


void CGraphicsView::handleRefreshGraphicsview(CDxfGraphicsScene* pScene, bool bResetViewRect)
{
    if (pScene)
    {
        //保留当前view的状态
        QTransform currentTransform = transform();
        QPointF currentCenter = mapToScene(viewport()->rect().center());
        int hValue = horizontalScrollBar()->value();
        int vValue = verticalScrollBar()->value();
        QRectF curRect = sceneRect();

        // 设置场景到视图
        setScene(pScene);

        if (bResetViewRect)
        {
            // 计算所有项目的边界矩形
            QRectF boundingRect;
            for (const auto& item : pScene->items())
            {
                boundingRect = boundingRect.united(item->boundingRect());
            }
            m_rectInitialScene = pScene->sceneRect();
            m_rectLastScene = pScene->sceneRect();
            QTimer::singleShot(0, [this, pScene]() {fitInView(pScene->sceneRect(), Qt::KeepAspectRatio); });
            // 强制更新视图
            update();
        }
        else
        {
            // 恢复之前的视图状态
            setSceneRect(curRect);
            setTransform(currentTransform);
            horizontalScrollBar()->setValue(hValue);
            verticalScrollBar()->setValue(vValue);
        }
    }
    return;
}

void CGraphicsView::mouseMoveEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mouseMoveEvent(pEvent);

    if (m_bShowMousePos && m_pLabelMousePos)
    {
        QPointF posScene = mapToScene(pEvent->pos());
        QString strPos = QString("X:%1 Y:%2").arg(posScene.x(),0,'f',1).arg(posScene.y(),0,'f',1);
        m_pLabelMousePos->setText(strPos);
        m_pLabelMousePos->adjustSize();
        // 计算标签位置，考虑边界限制
        QPoint posLabel = pEvent->pos() + QPoint(15, -20);
        QRect labelRect = m_pLabelMousePos->rect();
        QRect viewRect = viewport()->rect();

        // 检查右边界
        if (posLabel.x() + labelRect.width() > viewRect.right())
            posLabel.setX(pEvent->pos().x() - labelRect.width() - 15);
        // 检查上边界
        if (posLabel.y() < viewRect.top())
            posLabel.setY(pEvent->pos().y() + 20);

        m_pLabelMousePos->move(posLabel);
    }

    if (m_bDrag && (pEvent->buttons() & Qt::LeftButton))
    {
        QPoint delta = pEvent->pos() - m_pointLastPos;
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        hBar->setValue(hBar->value() - delta.x());
        vBar->setValue(vBar->value() - delta.y());
        m_pointLastPos = pEvent->pos();
    }
}

void CGraphicsView::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton && m_bDrag)
    {
        m_pointLastPos = pEvent->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsView::mousePressEvent(pEvent);
}

void CGraphicsView::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton && m_bDrag)
    {
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsView::mouseReleaseEvent(pEvent);
}
