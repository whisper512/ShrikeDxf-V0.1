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
    m_pGraphicsPreviewMenu(nullptr),
    m_pActionLockZoom(nullptr),
    m_pActionFilpX(nullptr),
    m_pActionFilpY(nullptr),
    m_pActionResetView(nullptr),
    m_pActionDrag(nullptr),
    m_pActionPasteEntity(nullptr),
    m_pActionEndDrawing(nullptr),
    m_pRulerH(nullptr),
    m_pRulerV(nullptr),
    m_bLockZoom(false),
    m_bFilpAlongX(false),
    m_bFilpAlongY(false),
    m_bShowPosCross(false),
    m_bDrag(false),
    m_bCopyingEntity(false),
    m_bDrawingPreview(false),
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
        setRenderHint(QPainter::Antialiasing);
        setStyleSheet(BACKGROUND_WHITE);
        //翻转y轴
        setTransform(m_tranformInitial);
        //确保视图可见
        setVisible(true);
        show();
    }
    //开启鼠标追踪
    setMouseTracking(true);
    InitMenu(this);
    InitRuler();
    InitScene();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CGraphicsView::InitMenu(QWidget* pParent)
{
    m_pGraphicsViewMenu = new QMenu();
    m_pGraphicsOperateMenu = new QMenu();
    m_pGraphicsPreviewMenu = new QMenu();

    setContextMenuPolicy(Qt::CustomContextMenu);
    InitGraphicsViewAction();
    connect(this, &QWidget::customContextMenuRequested, this, &CGraphicsView::ShowMenu);
}

void CGraphicsView::InitRuler()
{
    // 创建标尺
    m_pRulerH = new CRulerH(this);
    m_pRulerH->setFixedHeight(20);
    m_pRulerH->setGeometry(20, 0, width() - 20, 20);
    m_pRulerH->raise();
    m_pRulerH->show();

    m_pRulerV = new CRulerV(this);
    m_pRulerV->setFixedWidth(20);
    m_pRulerV->setGeometry(0, 0, 20, height());
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
    else if (m_bDrawingPreview)
    {
        m_pGraphicsPreviewMenu->popup(mapToGlobal(pos));
    }
    else
    {
        m_pGraphicsViewMenu->popup(mapToGlobal(pos));
    }
}


void CGraphicsView::InitGraphicsViewAction()
{
    //************
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

    m_pGraphicsViewMenu->addAction(m_pActionDrag);
    m_pGraphicsViewMenu->addAction(m_pActionLockZoom);
    m_pGraphicsViewMenu->addAction(m_pActionFilpX);
    m_pGraphicsViewMenu->addAction(m_pActionFilpY);
    m_pGraphicsViewMenu->addAction(m_pActionResetView);

    connect(m_pActionDrag, &QAction::toggled, this, &CGraphicsView::handleDrag);
    connect(m_pActionLockZoom, &QAction::toggled, this, &CGraphicsView::handleLockZoom);
    connect(m_pActionFilpX, &QAction::toggled, this, &CGraphicsView::handleFilpAlongX);
    connect(m_pActionFilpY, &QAction::toggled, this, &CGraphicsView::handleFilpAlongY);
    connect(m_pActionResetView, &QAction::triggered, this, &CGraphicsView::handleResetView);

    //************
    m_pActionPasteEntity = new QAction("Paste", this);
    m_pGraphicsOperateMenu->addAction(m_pActionPasteEntity);
    connect(m_pActionPasteEntity,&QAction::triggered,this,&CGraphicsView::handlePasteEntity);

    //************
    m_pActionEndDrawing = new QAction("End Drawing", this);
    m_pGraphicsPreviewMenu->addAction(m_pActionEndDrawing);
    connect(m_pActionEndDrawing, &QAction::triggered, this, &CGraphicsView::handleEndDrawingPreview);
}


void CGraphicsView::InitScene()
{
    //在没有加载scene的情况下，初始化一个scene
    if (!scene())
    {
        CDxfGraphicsScene* pScene;
        pScene = new CDxfGraphicsScene(this);
        pScene->setSceneRect(500, 500,500, 500);
        setScene(pScene);
    }
    UpdateRulers();
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

void CGraphicsView::handleEndDrawingPreview()
{
    m_bDrawingPreview = false;
    emit signalEndDrawingPreview();
}

void CGraphicsView::handleStartPreviewEntity(int index)
{
    if (index != -1)
    {
        m_bDrawingPreview = true;
    }
    else
    {
        m_bDrawingPreview = false;
    }
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
        UpdateRulers();
    }
}


void CGraphicsView::handleDrag(bool bChecked)
{
    m_bDrag = bChecked;
}


void CGraphicsView::handleRefreshGraphicsview(CDxfGraphicsScene* pScene, bool bResetViewTransform)
{
    if (pScene)
    {
        // 保留当前view的状态
        QTransform currentTransform = transform();
        QPointF currentCenter = mapToScene(viewport()->rect().center());
        int hValue = horizontalScrollBar()->value();
        int vValue = verticalScrollBar()->value();
        // 设置场景到视图
        setScene(pScene);
        if (bResetViewTransform)
        {
            // 重置视口到场景边界
            m_rectInitialScene = pScene->sceneRect();
            m_rectLastScene = pScene->sceneRect();
            QTimer::singleShot(0, [this, pScene]() {
                fitInView(pScene->sceneRect(), Qt::KeepAspectRatio);
                UpdateRulers();
                });
        }
        else
        {
            setTransform(currentTransform);
            centerOn(currentCenter);
            
            horizontalScrollBar()->setValue(hValue);
            verticalScrollBar()->setValue(vValue);
            UpdateRulers();
        }
        // 强制更新视图
        update();
    }
}

void CGraphicsView::mouseMoveEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mouseMoveEvent(pEvent);
    update();

    QPointF posScene = mapToScene(pEvent->pos());
    QString strPos = QString("X:%1 Y:%2").arg(posScene.x(),0,'f',3).arg(posScene.y(),0,'f',3);
    emit signalMousePosString(strPos);
    emit signalMousePos(posScene);
    emit signalGraphicsViewMouseMove(posScene);

    if (m_bDrag && (pEvent->buttons() & Qt::LeftButton))
    {
        QPoint delta = pEvent->pos() - m_pointLastPos;
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        hBar->setValue(hBar->value() - delta.x());
        vBar->setValue(vBar->value() - delta.y());
        m_pointLastPos = pEvent->pos();
    }

    //传递scene中的坐标
    QPointF scenePos = mapToScene(pEvent->pos());
    if (m_pRulerH)
    {
        m_pRulerH->SetMousePos(scenePos.x());
    }
    if (m_pRulerV)
    {
        m_pRulerV->SetMousePos(scenePos.y());
    }
}

void CGraphicsView::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton && m_bDrag)
    {
        m_pointLastPos = pEvent->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    if (pEvent->button() == Qt::LeftButton)
    {
        QPointF scenePos = mapToScene(pEvent->pos());
        emit signalGraphicsViewLeftCLick(scenePos);
    }
    if (pEvent->button() == Qt::RightButton)
    {
        QPointF scenePos = mapToScene(pEvent->pos());
        emit signalGraphicsViewRightClick(scenePos);
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
    QRectF sceneRect;
    QRectF viewRect;
    sceneRect = mapToScene(viewport()->rect()).boundingRect();
    
    QTransform transform = this->transform();
    // 设置标尺的范围为视图的X范围
    if (m_pRulerH)
    {
        m_pRulerH->SetRange(sceneRect.left(), sceneRect.right());
        m_pRulerH->SetOrigin(sceneRect.left());

        double scaleX = transform.m11();
        // 设置标尺的缩放比例
        m_pRulerH->SetRulerZoom(scaleX);
        m_pRulerH->update();
    }

    if (m_pRulerV)
    {
        double rulerTop = sceneRect.top();
        double rulerBottom = sceneRect.bottom();

        m_pRulerV->SetRange(rulerTop, rulerBottom);
        m_pRulerV->SetOrigin(rulerTop);
        double scaleY = transform.m22();
        // 设置标尺的缩放比例
        m_pRulerV->SetRulerZoom(scaleY);
        m_pRulerV->update();
    }
}


void CGraphicsView::resizeEvent(QResizeEvent* pEvent)
{
    QGraphicsView::resizeEvent(pEvent);

    if (m_pRulerH)
    {
        // 设置视口边距，为标尺预留空间
        setViewportMargins(20, 20, 0, 0);

        // 设置标尺位置
        m_pRulerH->setGeometry(20, 0, width()- 20, 20);
        m_pRulerH->raise();
        m_pRulerH->show();

        m_pRulerV->setGeometry(0, 0, 20, height());
        m_pRulerV->raise();
        m_pRulerV->show();

        // 更新标尺
        UpdateRulers();
    }
}

void CGraphicsView::drawForeground(QPainter* pPainter, const QRectF& rect)
{
    QGraphicsView::drawForeground(pPainter, rect);
    if (m_bShowPosCross)
    {

    }
}
