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
    m_pDataManager(nullptr),
	m_pDxfDataManger(nullptr),
	m_pMoveBtnsWidget(nullptr)
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

		
		if (ui.verticalLayout_Layer)
		{
			ui.verticalLayout_Layer->setStretch(0, 3);
			ui.verticalLayout_Layer->setStretch(1, 1);
			ui.verticalLayout_Layer->setStretch(2, 2);
		}
	});
}

void ShrikeDxf::InitDataManagers()
{
	QTimer::singleShot(0, this, [this]()
	{
		m_pDataManager = new CCommonDataManager(this);
		m_pDxfDataManger = new CDxfManger(this);
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
	m_pMoveBtnsWidget = new CMoveBtnsWidget(this);
	if (m_pMoveBtnsWidget)
	{
        m_pMoveBtnsWidget->InitWidgetAndAddToLayout();
	}
}

void ShrikeDxf::InitLabels()
{
	m_pLabelDocName = new QLabel(this);
	m_pLabelDocName->setText("DocName:None");
	m_pLabelDocName->setStyleSheet("QLabel {border: 5px solid transparent;; }");
	statusBar()->addWidget(m_pLabelDocName);

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
				connect(m_pDxfDataManger, &CDxfManger::signalRefreshTreeview, m_pTreeViewManger, &CTreeViewManger::handleRefreshTree);
				connect(m_pDxfDataManger, &CDxfManger::signalReturnEntityInfo, m_pTreeViewManger, &CTreeViewManger::handleReturnEntityInfo);
				connect(m_pTreeViewManger, &CTreeViewManger::signalSaveSelectedEntity, m_pDxfDataManger, &CDxfManger::handleSaveSelectedEntity);
				connect(m_pTreeViewManger,&CTreeViewManger::signalDeleteEntityData,m_pDxfDataManger, &CDxfManger::handleDeleteEntity);
				connect(m_pTreeViewManger, &CTreeViewManger::signalCopyEntityData, m_pDxfDataManger, &CDxfManger::handleCopyEntity);
				connect(m_pDxfDataManger, &CDxfManger::signalRefreshTreeviewAfterRead, m_pTreeViewManger, &CTreeViewManger::handleRefreshTreeviewAfterRead);
			}
			if (m_pDxfDataManger && m_pGraphicsView)
			{
				connect(m_pDxfDataManger, &CDxfManger::signalRefreshGraphicsview, m_pGraphicsView, &CGraphicsView::handleRefreshGraphicsview);
				//manger通知view正在进行复制,切换menu
				connect(m_pDxfDataManger, &CDxfManger::signalCopyintEntity, m_pGraphicsView, &CGraphicsView::handlelCopyintEntity);
				//view通知manger鼠标位置
				connect(m_pGraphicsView,&CGraphicsView::signalPaste,m_pDxfDataManger, &CDxfManger::handlePaste);
			}
			if (m_pDxfDataManger && m_pStackedWidgetManger)
			{

			}
			if (m_pDxfDataManger && m_pLayerTableViewManger)
			{
				connect(m_pDxfDataManger, &CDxfManger::signalRefreshLayerTableview, m_pLayerTableViewManger, &CLayerTableViewManger::handleRefreshLayerTableview);
				connect(m_pLayerTableViewManger, &CLayerTableViewManger::signalLayerModelChanged, m_pDxfDataManger, &CDxfManger::handleLayerModelChanged);
			}
			if (m_pDxfDataManger && m_pStackedWidgetManger)
			{
				connect(m_pDxfDataManger, &CDxfManger::signalRefreshStackedWidget, m_pStackedWidgetManger, &CStackedWidgetManger::handleRefreshStackedWidget);
				connect(m_pStackedWidgetManger->m_pPointAttributeWidget, &CPointAttributeWidget::signalPointAttributeChanged, m_pDxfDataManger, &CDxfManger::handlePointAttributeChanged);
				connect(m_pStackedWidgetManger->m_pLineAttributeWidget, &CLineAttributeWidget::signalLineAttributeChanged, m_pDxfDataManger, &CDxfManger::handleLineAttributeChanged);
				connect(m_pStackedWidgetManger->m_pCircleAttributeWidget, &CCircleAttributeWidget::signalCircleAttributeChanged, m_pDxfDataManger, &CDxfManger::handleCircleAttributeChanged);
				connect(m_pStackedWidgetManger->m_pArcAttributeWidget, &CArcAttritubeWidget::signalArcAttributeChanged, m_pDxfDataManger, &CDxfManger::handleArcAttributeChanged);
				connect(m_pStackedWidgetManger->m_pPolylineAttributeWidget, &CPolylineAttributeWidget::signalPolylineAttributeChanged, m_pDxfDataManger, &CDxfManger::handlePolylineAttributeChanged);
			}
			if (m_pDxfDataManger && m_pMoveBtnsWidget)
			{
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnUpClicked, m_pDxfDataManger, &CDxfManger::handleOnBtnUpClicked);
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnDownClicked, m_pDxfDataManger, &CDxfManger::handleOnBtnDownClicked);
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnLeftClicked, m_pDxfDataManger, &CDxfManger::handleOnBtnLeftClicked);
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnRightClicked, m_pDxfDataManger, &CDxfManger::handleOnBtnRightClicked);
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnCWClicked,m_pDxfDataManger, &CDxfManger::handleOnBtnCWClicked);
                connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnBtnCCWClicked, m_pDxfDataManger, &CDxfManger::handleOnBtnCCWClicked);
				connect(m_pMoveBtnsWidget, &CMoveBtnsWidget::signalOnStepLengthOrAngleChanged, m_pDxfDataManger, &CDxfManger::handleOnStepLengthOrAngleChanged);
				connect(m_pDxfDataManger, &CDxfManger::signalSetStepLengthAndAngle, m_pMoveBtnsWidget, &CMoveBtnsWidget::handleSetStepLengthAndAngle);
				connect(m_pDxfDataManger,&CDxfManger::signalSelectedEntityType,m_pMoveBtnsWidget,&CMoveBtnsWidget::handleSetBtnEnabled);
			}
			if (this && m_pGraphicsView)
			{
				connect(m_pGraphicsView, &CGraphicsView::signalMousePos, this, &ShrikeDxf::handlesignalMousePos);
			}
			if (this && m_pDxfDataManger)
			{
                connect(m_pDxfDataManger,&CDxfManger::signalShowFileName, this, &ShrikeDxf::handleShowDocName);
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

void ShrikeDxf::handlesignalMousePos(QString strPos)
{
	if (m_pLabelPos)
	{
		m_pLabelPos->setText(strPos);
	}
}
