#include "ShrikeDxf.h"


ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent),
	m_pMenuManger(nullptr),
	m_pTreeViewManger(nullptr),
	m_pGraphicsView(nullptr),
	m_pStackedWidgetManger(nullptr),
    m_pLayerTableViewManger(nullptr),
	m_pLabelPos(nullptr),
	m_pLabelDocName(nullptr),
	m_pLabelLayer(nullptr),
    m_pDataManager(nullptr),
	m_pDxfDataManger(nullptr),
	m_pMoveBtnsWidget(nullptr),
	m_pCreateEntityWidget(nullptr)
{	
    ui.setupUi(this);
	setWindowIcon(QIcon(":/ShrikeDxf/res/Main.png"));
	InitWindowComponents();
	InitDataManagers();
	InitLabels();
	ConnectSignalsAndSlots();
}

ShrikeDxf::~ShrikeDxf()
{
	delete m_pMenuManger;
}

void ShrikeDxf::InitWindowComponents()
{
	//初始化组件需要在确定ui界面加载完毕后进行,在进入消息循环后定时器开启执行初始化
	QTimer::singleShot(0, this, [this]()
	{
		InitAndCreateMenuBar();
		InitAndCreateTreeView();
		InitAndCreateGraphicsView();
		InitAndCreateStackedWidget();
		InitAndCreateMoveBtnsWidget();
		InitAndCreateLayerTableView();
		InitAndCreateCreateEntityWidget();

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

void ShrikeDxf::InitDataManagers()
{
	QTimer::singleShot(0, this, [this]()
	{
		m_pDataManager = new CCommonDataManager(this);
		m_pDxfDataManger = new CDxfManager(this);
	});
}


void ShrikeDxf::InitAndCreateMenuBar()
{
	m_pMenuManger = new CMenuManger(this);
	if (m_pMenuManger)
	{
		m_pMenuManger->InitMenuBar();
	}
}

void ShrikeDxf::InitAndCreateTreeView()
{
	m_pTreeViewManger = new CTreeViewManger(this);
	if (m_pTreeViewManger)
	{
		m_pTreeViewManger->CreateTreeView();
	}
}

void ShrikeDxf::InitAndCreateGraphicsView()
{
	m_pGraphicsView = new CGraphicsView(this);
}

void ShrikeDxf::InitAndCreateStackedWidget()
{
	m_pStackedWidgetManger = new CStackedWidgetManger(this);
	if (m_pStackedWidgetManger)
	{
        m_pStackedWidgetManger->CreateStackedWidget();
	}
}

void ShrikeDxf::InitAndCreateLayerTableView()
{
	m_pLayerTableViewManger = new CLayerTableViewManger(this);
	if (m_pLayerTableViewManger)
	{
		m_pLayerTableViewManger->CreateTableView();
	}
}

void ShrikeDxf::InitAndCreateMoveBtnsWidget()
{
}

void ShrikeDxf::InitAndCreateCreateEntityWidget()
{
	m_pCreateEntityWidget =  new CCreateEntityWidget(this);
	if (m_pCreateEntityWidget)
	{
        m_pCreateEntityWidget->InitWidgetAndAddToLayout();
		ui.horizontalLayout_FileStructure->setStretch(9, 1);
	}
}

void ShrikeDxf::InitLabels()
{
	m_pLabelDocName = new QLabel(this);
	m_pLabelDocName->setText("DocName:None");
	m_pLabelDocName->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_pLabelDocName);

	m_pLabelLayer = new QLabel(this);
	m_pLabelLayer->setText("Layer:None");
	m_pLabelLayer->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_pLabelLayer);

	m_pLabelPos = new QLabel(this);
	m_pLabelPos->setText("X:0.000 Y:0.000");
	m_pLabelPos->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_pLabelPos);

}


void ShrikeDxf::ConnectSignalsAndSlots()
{
	QTimer::singleShot(0, this, [this]()
		{
			if (m_pDxfDataManger && m_pTreeViewManger)
			{
				connect(m_pDxfDataManger, &CDxfManager::signalRefreshTreeview, m_pTreeViewManger, &CTreeViewManger::handleRefreshTree);
				connect(m_pDxfDataManger, &CDxfManager::signalRefreshTreeviewAfterRead, m_pTreeViewManger, &CTreeViewManger::handleRefreshTreeviewAfterRead);
				connect(m_pTreeViewManger, &CTreeViewManger::signalEntitySelected, m_pDxfDataManger, &CDxfManager::OnTreeViewEntitySelected);
				connect(m_pDxfDataManger, &CDxfManager::signalRefreshLayerTable, m_pLayerTableViewManger, &CLayerTableViewManger::handleRefreshLayerTableview);

				//connect(m_pDxfDataManger, &CDxfManger::signalReturnEntityInfo, m_pTreeViewManger, &CTreeViewManger::handleReturnEntityInfo);
				//connect(m_pTreeViewManger, &CTreeViewManger::signalSaveSelectedEntity, m_pDxfDataManger, &CDxfManger::handleSaveSelectedEntity);
				//connect(m_pTreeViewManger,&CTreeViewManger::signalDeleteEntityData,m_pDxfDataManger, &CDxfManger::handleDeleteEntity);
				//connect(m_pTreeViewManger, &CTreeViewManger::signalCopyEntityData, m_pDxfDataManger, &CDxfManger::handleCopyEntity);
			}
			if (m_pDxfDataManger && m_pGraphicsView)
			{
				connect(m_pDxfDataManger, &CDxfManager::signalRefreshGraphicsview, m_pGraphicsView, &CGraphicsView::handleRefreshGraphicsview);
				////manger通知view正在进行复制,切换menu
				//connect(m_pDxfDataManger, &CDxfManger::signalCopyintEntity, m_pGraphicsView, &CGraphicsView::handlelCopyintEntity);
				////view通知manger鼠标位置
				//connect(m_pGraphicsView,&CGraphicsView::signalPaste,m_pDxfDataManger, &CDxfManger::handlePaste);
				//connect(m_pGraphicsView, &CGraphicsView::signalGraphicsViewMouseMove,m_pDxfDataManger,&CDxfManger::handleGraphicsViewMouseMove);
				//connect(m_pGraphicsView, &CGraphicsView::signalGraphicsViewLeftCLick,m_pDxfDataManger, &CDxfManger::handleGraphicsViewLeftClick);
				//connect(m_pGraphicsView, &CGraphicsView::signalGraphicsViewRightClick, m_pDxfDataManger, &CDxfManger::handleGraphicsViewRightClick);
				////preview相关
				//connect(m_pDxfDataManger,&CDxfManger::signalStartPreviewEntity,m_pGraphicsView,&CGraphicsView::handleStartPreviewEntity);
				//connect(m_pGraphicsView,&CGraphicsView::signalEndDrawingPreview,m_pDxfDataManger, &CDxfManger::handleEndDrawPreview);
			}
			if (m_pDxfDataManger && m_pStackedWidgetManger)
			{
				connect(m_pDxfDataManger, &CDxfManager::signalSelectedEntityChanged, m_pStackedWidgetManger, &CStackedWidgetManger::handleRefreshStackedWidget);
			}
			if (m_pDxfDataManger && m_pLayerTableViewManger)
			{
				//connect(m_pDxfDataManger, &CDxfManger::signalRefreshLayerTableview, m_pLayerTableViewManger, &CLayerTableViewManger::handleRefreshLayerTableview);
				//connect(m_pLayerTableViewManger, &CLayerTableViewManger::signalLayerModelChanged, m_pDxfDataManger, &CDxfManger::handleLayerModelChanged);
				//connect(m_pLayerTableViewManger, &CLayerTableViewManger::signalChangeCurrentLayer, m_pDxfDataManger, &CDxfManger::handleChangeCurrentLayer);
			}
			if (m_pDxfDataManger && m_pStackedWidgetManger)
			{
				connect(m_pStackedWidgetManger, &CStackedWidgetManger::signalPointChanged, m_pDxfDataManger, &CDxfManager::handlePointAttributeChanged);
				connect(m_pStackedWidgetManger, &CStackedWidgetManger::signalLineChanged, m_pDxfDataManger, &CDxfManager::handleLineAttributeChanged);
				connect(m_pStackedWidgetManger, &CStackedWidgetManger::signalCircleChanged, m_pDxfDataManger, &CDxfManager::handleCircleAttributeChanged);
				connect(m_pStackedWidgetManger, &CStackedWidgetManger::signalArcChanged, m_pDxfDataManger, &CDxfManager::handleArcAttributeChanged);
				
			}
			if (m_pDxfDataManger && m_pMoveBtnsWidget)
			{

			}
			if (this && m_pGraphicsView)
			{
				connect(m_pGraphicsView, &CGraphicsView::signalMousePos, this, &ShrikeDxf::handleMousePos);
			}
			if (this && m_pDxfDataManger)
			{
                connect(m_pDxfDataManger,&CDxfManager::signalFileName, this, &ShrikeDxf::handleShowDocName);
				//connect(m_pDxfDataManger,&CDxfManger::signalCurrentLayer,this,&ShrikeDxf::handleShowLayerName);
			}
			if (m_pCreateEntityWidget && m_pDxfDataManger)
			{
				//connect(m_pCreateEntityWidget,&CCreateEntityWidget::signalMouseStatus,m_pDxfDataManger, &CDxfManger::handleMouseStatus);
				//connect(m_pDxfDataManger,&CDxfManger::signalChangeCreateBtnStatus,m_pCreateEntityWidget,&CCreateEntityWidget::handleChangeCreateBtnStatus);
			}
			if (m_pCreateEntityWidget && m_pGraphicsView)
			{
				
			}

		});
}

void ShrikeDxf::handleShowDocName(QString strDocName)
{
	if (!strDocName.isEmpty())
	{
		QFileInfo fileInfo(strDocName);
		QString fileName = fileInfo.fileName();
		if (m_pLabelDocName)
		{
			m_pLabelDocName->setText("DocName:" + fileName);
		}
	}
}

void ShrikeDxf::handleShowLayerName(QString strLayerName)
{
	if (m_pLabelLayer)
	{
		m_pLabelLayer->setText("Layer:" + strLayerName);
	}
}

void ShrikeDxf::handleMousePos(QString strPos)
{
	if (m_pLabelPos)
	{
		m_pLabelPos->setText(strPos);
	}
}
