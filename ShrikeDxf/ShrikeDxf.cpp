#include "ShrikeDxf.h"


ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent),
	m_menuManger(nullptr),
	m_treeViewManger(nullptr),
	m_graphicsView(nullptr),
	m_stackedWidgetManager(nullptr),
    m_layerTableViewManager(nullptr),
	m_labelPos(nullptr),
	m_labelDocName(nullptr),
	m_labelLayer(nullptr),
    m_pDataManager(nullptr),
	m_dxfDataManager(nullptr),
	m_moveBtnsWidget(nullptr),
	m_createEntityWidget(nullptr)
{	
    ui.setupUi(this);
	setWindowIcon(QIcon(":/ShrikeDxf/res/Main.png"));
	initDataManagers();
	initWindowComponents();
	initLabels();
	connectSignalsAndSlots();
}

ShrikeDxf::~ShrikeDxf()
{
	delete m_menuManger;
}

void ShrikeDxf::initWindowComponents()
{
	//初始化组件需要在确定ui界面加载完毕后进行,在进入消息循环后定时器开启执行初始化
	QTimer::singleShot(0, this, [this]()
	{
		initAndCreateMenuBar();
		initAndCreateTreeView();
		initAndCreateGraphicsView();
		initAndCreateStackedWidget();
		initAndCreateMoveBtnsWidget();
		initAndCreateLayerTableView();
		initAndCreateCreateEntityWidget();

		if (ui.verticalLayout_Layer)
		{
			ui.verticalLayout_Layer->setStretch(0, 3);
			ui.verticalLayout_Layer->setStretch(1, 1);
			ui.verticalLayout_Layer->setStretch(2, 2);
		}
		if (ui.horizontalLayout_FileStructure)
		{
			ui.horizontalLayout_FileStructure->setSpacing(2);
			ui.horizontalLayout_FileStructure->setContentsMargins(0, 0, 0, 0);
            ui.horizontalLayout_FileStructure->setStretch(0, 9);
            ui.horizontalLayout_FileStructure->setStretch(1, 1);
		}
	});
}

void ShrikeDxf::initDataManagers()
{
	QTimer::singleShot(0, this, [this]()
	{
		m_pDataManager = new CommonDataManager(this);
		m_dxfDataManager = new DxfManager(this);
	});
}


void ShrikeDxf::initAndCreateMenuBar()
{
	m_menuManger = new MenuManager(this);
	if (m_menuManger)
	{
		m_menuManger->InitMenuBar();
	}
}

void ShrikeDxf::initAndCreateTreeView()
{
	m_treeViewManger = new TreeViewManager(this);
	if (m_treeViewManger)
	{
		m_treeViewManger->CreateTreeView();
	}
}

void ShrikeDxf::initAndCreateGraphicsView()
{
	m_graphicsView = new GraphicsView(this, m_dxfDataManager);
}

void ShrikeDxf::initAndCreateStackedWidget()
{
	m_stackedWidgetManager = new StackedWidgetManager(this);
	if (m_stackedWidgetManager)
	{
        m_stackedWidgetManager->CreateStackedWidget();
	}
}

void ShrikeDxf::initAndCreateLayerTableView()
{
	m_layerTableViewManager = new LayerTableViewManager(this);
	if (m_layerTableViewManager)
	{
		m_layerTableViewManager->CreateTableView();
	}
}

void ShrikeDxf::initAndCreateMoveBtnsWidget()
{
}

void ShrikeDxf::initAndCreateCreateEntityWidget()
{
	m_createEntityWidget =  new CreateEntityWidget(this);
	if (m_createEntityWidget)
	{
        m_createEntityWidget->InitWidgetAndAddToLayout();
		ui.horizontalLayout_FileStructure->setStretch(9, 1);
	}
}

void ShrikeDxf::initLabels()
{
	m_labelDocName = new QLabel(this);
	m_labelDocName->setText("DocName:None");
	m_labelDocName->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_labelDocName);

	m_labelLayer = new QLabel(this);
	m_labelLayer->setText("Layer:None");
	m_labelLayer->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_labelLayer);

	m_labelPos = new QLabel(this);
	m_labelPos->setText("X:0.000 Y:0.000");
	m_labelPos->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_labelPos);

}


void ShrikeDxf::connectSignalsAndSlots()
{
	QTimer::singleShot(0, this, [this]()
		{
			if (m_dxfDataManager && m_treeViewManger)
			{
				connect(m_dxfDataManager, &DxfManager::signalRefreshTreeview, m_treeViewManger, &TreeViewManager::handleRefreshTree);
				connect(m_dxfDataManager, &DxfManager::signalRefreshTreeviewAfterRead, m_treeViewManger, &TreeViewManager::handleRefreshTreeviewAfterRead);
				connect(m_treeViewManger, &TreeViewManager::signalEntitySelected, m_dxfDataManager, &DxfManager::handleEntitySelected);
				
			}
			if (m_dxfDataManager && m_graphicsView)
			{
				connect(m_dxfDataManager, &DxfManager::signalRefreshGraphicsview, m_graphicsView, &GraphicsView::handleRefreshGraphicsview);
				connect(m_graphicsView, &GraphicsView::signalMousePos, m_dxfDataManager, &DxfManager::handleMousePos);
				connect(m_graphicsView, &GraphicsView::signalGraphicsViewLeftCLick, m_dxfDataManager, &DxfManager::handleMouseLeftButtonClicked);
				connect(m_graphicsView, &GraphicsView::signalGraphicsViewRightClick, m_dxfDataManager, &DxfManager::handleMouseRightButtonClicked);
				connect(m_graphicsView, &GraphicsView::signalGraphicsViewLeftPressed,m_dxfDataManager, &DxfManager::handleMouseLeftButtonPressed);
				connect(m_graphicsView, &GraphicsView::signalGraphicsViewLeftReleased,m_dxfDataManager, &DxfManager::handleMouseLeftButtonReleased);

				connect(m_dxfDataManager, &DxfManager::signalMouseStatusChanged, m_graphicsView, &GraphicsView::handleMouseStatusChanged);
				connect(m_graphicsView, &GraphicsView::signalEndDrawingPreview, m_dxfDataManager, &DxfManager::handleEndDrawingPreview);
			}
			if (m_dxfDataManager && m_stackedWidgetManager)
			{
				connect(m_dxfDataManager, &DxfManager::signalSelectedEntityChanged, m_stackedWidgetManager, &StackedWidgetManager::handleRefreshStackedWidget);
			}
			if (m_dxfDataManager && m_layerTableViewManager)
			{
				connect(m_dxfDataManager, &DxfManager::signalRefreshLayerTable, m_layerTableViewManager, &LayerTableViewManager::handleRefreshLayerTableview);
				connect(m_layerTableViewManager, &LayerTableViewManager::signalLayerModelChanged, m_dxfDataManager, &DxfManager::handleLayerAttributeChanged);
			}
			if (m_dxfDataManager && m_stackedWidgetManager)
			{
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalPointChanged, m_dxfDataManager, &DxfManager::handlePointAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalLineChanged, m_dxfDataManager, &DxfManager::handleLineAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalCircleChanged, m_dxfDataManager, &DxfManager::handleCircleAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalArcChanged, m_dxfDataManager, &DxfManager::handleArcAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalEllipseChanged, m_dxfDataManager, &DxfManager::handleEllipseAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalTextChanged, m_dxfDataManager, &DxfManager::handleTextAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalMTextChanged, m_dxfDataManager, &DxfManager::handleMTextAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalLWPolylineChanged, m_dxfDataManager, &DxfManager::handleLwpolylineAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalPolylineChanged, m_dxfDataManager, &DxfManager::handlePolylineAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalHatchChanged, m_dxfDataManager, &DxfManager::handleHatchAttributeChanged);
				connect(m_stackedWidgetManager, &StackedWidgetManager::signalSplineChanged, m_dxfDataManager, &DxfManager::handleSplineAttributeChanged);
			}
			if (this && m_graphicsView)
			{
				connect(m_graphicsView, &GraphicsView::signalMousePosString, this, &ShrikeDxf::handleMousePos);
				//connect(m_pGraphicsView, &CGraphicsView::signalDeleteEntity, this, &CDxfManager::DeleteSelectedEntity);
				//connect(m_pGraphicsView, &CGraphicsView::signalCopyEntity, this, &CDxfManager::CopySelectedEntity);
				//connect(m_pGraphicsView, &CGraphicsView::signalCutEntity, this, &CDxfManager::CutSelectedEntity);
				//connect(m_pGraphicsView, &CGraphicsView::signalPaste, this, &CDxfManager::PasteEntityAt);
			}
			if (this && m_dxfDataManager)
			{
                connect(m_dxfDataManager,&DxfManager::signalFileName, this, &ShrikeDxf::handleShowDocName);
				connect(m_dxfDataManager, &DxfManager::signalCurrentLayerChanged, this, &ShrikeDxf::handleShowLayerName);
			}
			if (m_createEntityWidget && m_dxfDataManager)
			{
				connect(m_createEntityWidget,&CreateEntityWidget::signalMouseStatus,m_dxfDataManager, &DxfManager::handleOnMouseStatusChanged);
			}
		});
}

void ShrikeDxf::handleShowDocName(QString strDocName)
{
	if (!strDocName.isEmpty())
	{
		QFileInfo fileInfo(strDocName);
		QString fileName = fileInfo.fileName();
		if (m_labelDocName)
		{
			m_labelDocName->setText("DocName:" + fileName);
		}
	}
}

void ShrikeDxf::handleShowLayerName(QString strLayerName)
{
	if (m_labelLayer)
	{
		m_labelLayer->setText("Layer:" + strLayerName);
	}
}

void ShrikeDxf::handleMousePos(QString strPos)
{
	if (m_labelPos)
	{
		m_labelPos->setText(strPos);
	}
}
