#include <QMessageBox>

#include "GraphicsView.h"
#include "StyleSheet.h"
#include "ShrikeDxf.h"
#include "../Manager/DxfManager.h" 

GraphicsView::GraphicsView()
{
}

GraphicsView::~GraphicsView()
{
}

GraphicsView::GraphicsView(QWidget* mainWnd, DxfManager* dxfManager):
    m_mainWnd(mainWnd),
    m_graphicsViewMenu(nullptr),
    m_graphicsOperateMenu(nullptr),
    m_graphicsPreviewMenu(nullptr),
    m_actionLockZoom(nullptr),
    m_actionFilpX(nullptr),
    m_actionFilpY(nullptr),
    m_actionResetView(nullptr),
    m_actionDrag(nullptr),
    m_actionDeleteEntity(nullptr),
    m_actionCopyEntity(nullptr),
    m_actionPasteEntity(nullptr),
    m_actionCutEntity(nullptr),
    m_actionXFile(nullptr),
    m_actionYFile(nullptr),
    m_actionEndDrawing(nullptr),
    m_rulerH(nullptr),
    m_rulerV(nullptr),
    m_lockZoom(false),
    m_filpAlongX(false),
    m_filpAlongY(false),
    m_showPosCross(false),
    m_drag(false),
    m_copyingEntity(false),
    m_drawingPreview(false),
    m_selectingEntity(false),
    m_tranformInitial(1, 0, 0, -1, 0, 0),
    m_rectInitialScene(0, 0, 0, 0),
    m_lastPos(0, 0),
    m_rectLastScene(0, 0, 0, 0),
    m_rightClickPos(0, 0),
    m_dxfManager(dxfManager)
{
    //添加graphicsview到layout
    ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_mainWnd);
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
    initMenu(this);
    initRuler();
    initScene();

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GraphicsView::initMenu(QWidget* pParent)
{
    m_graphicsViewMenu = new QMenu();
    m_graphicsOperateMenu = new QMenu();
    m_graphicsPreviewMenu = new QMenu();

    setContextMenuPolicy(Qt::CustomContextMenu);
    initGraphicsViewAction();
    connect(this, &QWidget::customContextMenuRequested, this, &GraphicsView::showMenu);
}

void GraphicsView::initRuler()
{
    // 创建标尺
    m_rulerH = new RulerH(this);
    m_rulerH->setFixedHeight(20);
    m_rulerH->setGeometry(20, 0, width() - 20, 20);
    m_rulerH->raise();
    m_rulerH->show();

    m_rulerV = new RulerV(this);
    m_rulerV->setFixedWidth(20);
    m_rulerV->setGeometry(0, 0, 20, height());
    m_rulerV->raise();
    m_rulerV->show();
}

void GraphicsView::showMenu(const QPoint& pos)
{
    m_rightClickPos = pos;
    m_rightClickPos = pos;
    // 获取交互控制的状态
    MouseStateInView state = m_dxfManager->getCurrentInteractionState();

    if (state >= MouseStateInView::Point &&
        state <= MouseStateInView::MText)
    {
        // 正在绘制,显示“结束绘制”
        m_graphicsPreviewMenu->popup(mapToGlobal(pos));
    }
    else if (state >= MouseStateInView::Move)
    {
        // 编辑状态(拉伸/移动等)
        m_graphicsOperateMenu->popup(mapToGlobal(pos));
    }
    else if (state == MouseStateInView::None)
    {
        bool hasSelection = (m_dxfManager->getSelectedEntity().entityIndex >= 0);
        bool hasClipboard = m_dxfManager->hasClipboard();

        if (hasSelection || hasClipboard)
        {
            // 根据状态启用 / 禁用菜单项
            m_actionDeleteEntity->setEnabled(hasSelection);
            m_actionCopyEntity->setEnabled(hasSelection);
            m_actionCutEntity->setEnabled(hasSelection);
            m_actionPasteEntity->setEnabled(hasClipboard);

            m_graphicsOperateMenu->popup(mapToGlobal(pos));
        }
        else
        {
            m_graphicsViewMenu->popup(mapToGlobal(pos));
        }
    }
}


void GraphicsView::initGraphicsViewAction()
{
    m_actionDrag = new QAction("Drag", this);
    m_actionDrag->setCheckable(true);
    m_actionDrag->setChecked(false);
    m_actionLockZoom = new QAction("Lock Zoom", this);
    m_actionLockZoom->setCheckable(true);
    m_actionLockZoom->setChecked(false);
    m_actionFilpX = new QAction("Horizontal Flip", this);
    m_actionFilpX->setCheckable(true);
    m_actionFilpX->setChecked(false);
    m_actionFilpY = new QAction("Vertical Flip", this);
    m_actionFilpY->setCheckable(true);
    m_actionFilpY->setChecked(false);
    m_actionResetView = new QAction("Reset View", this);

    m_graphicsViewMenu->addAction(m_actionDrag);
    m_graphicsViewMenu->addAction(m_actionLockZoom);
    m_graphicsViewMenu->addAction(m_actionFilpX);
    m_graphicsViewMenu->addAction(m_actionFilpY);
    m_graphicsViewMenu->addAction(m_actionResetView);

    connect(m_actionDrag, &QAction::toggled, this, &GraphicsView::handleDrag);
    connect(m_actionLockZoom, &QAction::toggled, this, &GraphicsView::handleLockZoom);
    connect(m_actionFilpX, &QAction::toggled, this, &GraphicsView::handleFilpAlongX);
    connect(m_actionFilpY, &QAction::toggled, this, &GraphicsView::handleFilpAlongY);
    connect(m_actionResetView, &QAction::triggered, this, &GraphicsView::handleResetView);

    m_actionEndDrawing = new QAction(QStringLiteral("End Drawing"), this);
    m_graphicsPreviewMenu->addAction(m_actionEndDrawing);

    connect(m_actionEndDrawing, &QAction::triggered, this, [this]() { emit signalEndDrawingPreview();});

    m_actionDeleteEntity = new QAction(QStringLiteral("Delete"), this);
    m_actionCopyEntity = new QAction(QStringLiteral("Copy"), this);
    m_actionCutEntity = new QAction(QStringLiteral("Cut"), this);
    m_actionPasteEntity = new QAction(QStringLiteral("Paste"), this);

    m_graphicsOperateMenu->addAction(m_actionDeleteEntity);
    m_graphicsOperateMenu->addAction(m_actionCopyEntity);
    m_graphicsOperateMenu->addAction(m_actionCutEntity);
    m_graphicsOperateMenu->addAction(m_actionPasteEntity);

        // 删除
        connect(m_actionDeleteEntity, &QAction::triggered, this, [this]() {
            if (m_dxfManager)
                m_dxfManager->deleteSelectedEntity();
            });

        // 复制
        connect(m_actionCopyEntity, &QAction::triggered, this, [this]() {
            if (m_dxfManager)
                m_dxfManager->copySelectedEntity();
            });

        // 剪切
        connect(m_actionCutEntity, &QAction::triggered, this, [this]() {
            if (m_dxfManager)
                m_dxfManager->cutSelectedEntity();
            });

        // 粘贴（场景坐标已在右键时记录为 m_rightClickPos）
        connect(m_actionPasteEntity, &QAction::triggered, this, [this]() {
            if (m_dxfManager)
                m_dxfManager->pasteEntity(mapToScene(m_rightClickPos));
            });
}


void GraphicsView::initScene()
{
    //在没有加载scene的情况下，初始化一个scene
    if (!scene())
    {
        DxfGraphicsScene* pScene;
        pScene = new DxfGraphicsScene(this);
        pScene->setSceneRect(500, 500,500, 500);
        setScene(pScene);
    }
    updateRulers();
}

void GraphicsView::filpView()
{
    QTransform curTransform = transform();
    // 当前缩放比例,只取绝对值
    double curScale = std::abs(curTransform.m11());

    QTransform transform;
    if (m_filpAlongX && m_filpAlongY)
    {
        // x轴和y轴都翻转
        transform = QTransform(-curScale, 0, 0, curScale, 0, 0);
    }
    else if (m_filpAlongX)
    {
        // 只翻转x轴
        transform = QTransform(-curScale, 0, 0, -curScale, 0, 0);
    }
    else if (m_filpAlongY)
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


void GraphicsView::handlelCopyintEntity()
{
    m_copyingEntity = true;
}

void GraphicsView::handleFilpAlongX(bool bChecked)
{
    m_filpAlongX = bChecked;
    filpView();
}

void GraphicsView::handleFilpAlongY(bool bChecked)
{
    m_filpAlongY = bChecked;
    filpView();
}


void GraphicsView::handleLockZoom(bool bChecked)
{
    m_lockZoom = bChecked;
}


void GraphicsView::handleMouseStatusChanged(MouseStateInView mouseState)
{
    m_drawingPreview = (mouseState != MouseStateInView::None);
}



void GraphicsView::wheelEvent(QWheelEvent* pEvent)
{
    if (m_lockZoom)
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
    updateRulers();
}

void GraphicsView::handleResetView()
{
    setTransform(m_tranformInitial);
    if (scene())
    {
        fitInView(m_rectInitialScene, Qt::KeepAspectRatio);
        updateRulers();
    }
}


void GraphicsView::handleDrag(bool bChecked)
{
    m_drag = bChecked;
}


void GraphicsView::handleRefreshGraphicsview(DxfGraphicsScene* pScene, bool bResetViewTransform)
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
                updateRulers();
                });
        }
        else
        {
            setTransform(currentTransform);
            centerOn(currentCenter);
            
            horizontalScrollBar()->setValue(hValue);
            verticalScrollBar()->setValue(vValue);
            updateRulers();
        }
        // 强制更新视图
        update();
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent* pEvent)
{
    QGraphicsView::mouseMoveEvent(pEvent);
    update();

    QPointF posScene = mapToScene(pEvent->pos());
    QString strPos = QString("X:%1 Y:%2").arg(posScene.x(),0,'f',3).arg(posScene.y(),0,'f',3);
    emit signalMousePosString(strPos);
    emit signalMousePos(posScene);
    emit signalGraphicsViewMouseMove(posScene);

    if (m_drag && (pEvent->buttons() & Qt::LeftButton))
    {
        QPoint delta = pEvent->pos() - m_lastPos;
        QScrollBar* hBar = horizontalScrollBar();
        QScrollBar* vBar = verticalScrollBar();
        hBar->setValue(hBar->value() - delta.x());
        vBar->setValue(vBar->value() - delta.y());
        m_lastPos = pEvent->pos();
    }

    //传递scene中的坐标
    QPointF scenePos = mapToScene(pEvent->pos());
    if (m_rulerH)
    {
        m_rulerH->setMousePos(scenePos.x());
    }
    if (m_rulerV)
    {
        m_rulerV->setMousePos(scenePos.y());
    }
}

void GraphicsView::mousePressEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton && m_drag)
    {
        m_lastPos = pEvent->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    if (pEvent->button() == Qt::LeftButton)
    {
        QPointF scenePos = mapToScene(pEvent->pos());
        emit signalGraphicsViewLeftCLick(scenePos);
        emit signalGraphicsViewLeftPressed(scenePos);
    }
    if (pEvent->button() == Qt::RightButton)
    {
        QPointF scenePos = mapToScene(pEvent->pos());
        emit signalGraphicsViewRightClick(scenePos);
    }

    QGraphicsView::mousePressEvent(pEvent);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent* pEvent)
{
    if (pEvent->button() == Qt::LeftButton && m_drag)
    {
        setCursor(Qt::ArrowCursor);
    }
    if (pEvent->button() == Qt::LeftButton)
    {
        QPointF scenePos = mapToScene(pEvent->pos());
        emit signalGraphicsViewLeftReleased(scenePos);
    }
    QGraphicsView::mouseReleaseEvent(pEvent);
}

void GraphicsView::updateRulers()
{
    // 获取当前视图在场景中的矩形
    QRectF sceneRect;
    QRectF viewRect;
    sceneRect = mapToScene(viewport()->rect()).boundingRect();
    
    QTransform transform = this->transform();
    // 设置标尺的范围为视图的X范围
    if (m_rulerH)
    {
        m_rulerH->setRange(sceneRect.left(), sceneRect.right());
        m_rulerH->setOrigin(sceneRect.left());

        double scaleX = transform.m11();
        // 设置标尺的缩放比例
        m_rulerH->setRulerZoom(scaleX);
        m_rulerH->update();
    }

    if (m_rulerV)
    {
        double rulerTop = sceneRect.top();
        double rulerBottom = sceneRect.bottom();

        m_rulerV->setRange(rulerTop, rulerBottom);
        m_rulerV->setOrigin(rulerTop);
        double scaleY = transform.m22();
        // 设置标尺的缩放比例
        m_rulerV->setRulerZoom(scaleY);
        m_rulerV->update();
    }
}


void GraphicsView::resizeEvent(QResizeEvent* pEvent)
{
    QGraphicsView::resizeEvent(pEvent);

    if (m_rulerH)
    {
        // 设置视口边距，为标尺预留空间
        setViewportMargins(20, 20, 0, 0);

        // 设置标尺位置
        m_rulerH->setGeometry(20, 0, width()- 20, 20);
        m_rulerH->raise();
        m_rulerH->show();

        m_rulerV->setGeometry(0, 0, 20, height());
        m_rulerV->raise();
        m_rulerV->show();

        // 更新标尺
        updateRulers();
    }
}

void GraphicsView::drawForeground(QPainter* pPainter, const QRectF& rect)
{
    QGraphicsView::drawForeground(pPainter, rect);
    if (m_showPosCross)
    {

    }
}
