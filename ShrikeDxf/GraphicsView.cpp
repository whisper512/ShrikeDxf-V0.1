#include <QMessageBox>

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
    m_pGraphicsOperateMenu(nullptr),
    m_pLabelMousePos(nullptr),
    m_pActionLockZoom(nullptr),
    m_pActionFilpX(nullptr),
    m_pActionFilpY(nullptr),
    m_pActionResetView(nullptr),
    m_pActionShowMousePos(nullptr),
    m_pActionDrag(nullptr),
    m_pActionPasteEntity(nullptr),
    m_pRulerH(nullptr),
    m_pRulerV(nullptr),
    m_bLockZoom(false),
    m_bFilpAlongX(false),
    m_bFilpAlongY(false),
    m_bShowMousePos(false),
    m_bDrag(false),
    m_bCopyingEntity(false),
    m_tranformInitial(1, 0, 0, -1, 0, 0),
    m_rectInitialScene(0, 0, 0, 0),
    m_pointLastPos(0, 0),
    m_rectLastScene(0, 0, 0, 0),
    m_pointRightClickPos(0, 0)
{
    //添加graphicsview到layout
    ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
    if (pWnd)
    {
        pWnd->ui.verticalLayout_Main->addWidget(this);
        setAutoFillBackground(true);
        setStyleSheet(BACKGROUND_GRAY);
        setRenderHint(QPainter::Antialiasing);
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

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CGraphicsView::InitMenu(QWidget* pParent)
{
    m_pGraphicsViewMenu = new QMenu();
    m_pGraphicsOperateMenu = new QMenu();

    setContextMenuPolicy(Qt::CustomContextMenu);
    InitGraphicsOperateAction();
    InitGraphicsViewAction();
    connect(this, &QWidget::customContextMenuRequested, this, &CGraphicsView::ShowMenu);

    // 创建标尺
    m_pRulerH = new CRulerH(this);
    m_pRulerH->setFixedHeight(20);
    m_pRulerH->setGeometry(0, 0, width(), 25);
    m_pRulerH->SetStepRange(1, 100.0);
    m_pRulerH->raise();
    m_pRulerH->show();

    m_pRulerV = new CRulerV(this);
    m_pRulerV->setFixedWidth(20);
    m_pRulerV->setGeometry(0, 25, 25, height() - 25); // 修改垂直标尺位置和高度
    m_pRulerV->SetStepRange(1, 100.0);
    m_pRulerV->raise();
    m_pRulerV->show();
}

void CGraphicsView::ShowMenu(const QPoint& pos)
{
    m_pointRightClickPos = pos;
    if (m_bCopyingEntity)
    {
        m_pGraphicsOperateMenu->popup(mapToGlobal(pos));
    }
    else
    {
        m_pGraphicsViewMenu->popup(mapToGlobal(pos));
    }
}


void CGraphicsView::InitGraphicsViewAction()
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

void CGraphicsView::InitGraphicsOperateAction()
{
    m_pActionPasteEntity = new QAction("Paste", this);

    m_pGraphicsOperateMenu->addAction(m_pActionPasteEntity);

    connect(m_pActionPasteEntity,&QAction::triggered,this,&CGraphicsView::handlePasteEntity);
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


void CGraphicsView::handlelCopyintEntity()
{
    m_bCopyingEntity = true;
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

void CGraphicsView::handlePasteEntity()
{
    //重置
    m_bCopyingEntity = false;
    emit signalPaste(mapToScene(m_pointRightClickPos));
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

    QRectF currentViewRect = mapToScene(viewport()->rect()).boundingRect();
    double newWidth = currentViewRect.width() / scaleFactorActual;
    double newHeight = currentViewRect.height() / scaleFactorActual;

    QPointF viewCenterScene = mapToScene(viewport()->rect().center());
    QPointF newCenter = mouseScenePos + (viewCenterScene - mouseScenePos) / scaleFactorActual;
    scale(scaleFactorActual, scaleFactorActual);

    // 设置新的视图中心
    centerOn(newCenter);

    // 更新标尺
    UpdateRulers();
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
            update();
            viewport()->update();
        }
    }
    UpdateRulers();
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

    QPoint viewPos = pEvent->pos();
    if (m_pRulerH)
    {
        m_pRulerH->SetMousePos(viewPos.x());
    }
    if (m_pRulerV)
    {
        m_pRulerV->SetMousePos(viewPos.y()-25);
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

void CGraphicsView::UpdateRulers()
{
    // 获取当前视图在场景中的矩形
    QRectF viewRect = mapToScene(viewport()->rect()).boundingRect();
    QTransform transform = this->transform();
    // 设置标尺的范围为视图的X范围
    if (m_pRulerH)
    {
        m_pRulerH->SetRange(viewRect.left(), viewRect.right());
        m_pRulerH->SetOrigin(viewRect.left());
        // 获取当前的缩放比例（X方向）
        double scaleX = transform.m11();
        // 设置标尺的缩放比例
        m_pRulerH->SetRulerZoom(scaleX);
    }

    if (m_pRulerV)
    {
        double rulerTop = viewRect.top();
        double rulerBottom = viewRect.bottom();

        m_pRulerV->SetRange(rulerTop, rulerBottom);
        m_pRulerV->SetOrigin(rulerTop);
        double scaleY = std::abs(transform.m22());
        // 设置标尺的缩放比例
        m_pRulerV->SetRulerZoom(scaleY);
    }

}

void CGraphicsView::resizeEvent(QResizeEvent* pEvent)
{
    QGraphicsView::resizeEvent(pEvent);

    if (m_pRulerH)
    {
        // 设置视口边距，为标尺预留空间
        setViewportMargins(25, 25, 0, 0);

        // 设置标尺位置
        m_pRulerH->setGeometry(0, 0, width(), 25);
        m_pRulerH->raise();
        m_pRulerH->show();

        m_pRulerV->setGeometry(0, 25, 25, height() - 25);
        m_pRulerV->raise();
        m_pRulerV->show();

        // 更新标尺
        UpdateRulers();
    }
}
