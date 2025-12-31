#include "ShrikeDxf.h"


ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent),
	m_pMenuManger(nullptr),
	m_pTreeViewManger(nullptr),
	m_pGraphicsView(nullptr),
	m_pStackedWidgetManger(nullptr),
    m_pTableViewManger(nullptr),
    m_pDataManager(nullptr),
	m_pDxfDataManger(nullptr)
{	
    ui.setupUi(this);
	setWindowIcon(QIcon(":/ShrikeDxf/res/Main.png"));

	InitWindowComponents();
	InitDataManagers();
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
		InitMenuBar();
		InitTreeView();
		InitGraphicsView();
		InitStackedWidget();
		InitTableView();
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


void ShrikeDxf::InitMenuBar()
{
	m_pMenuManger = new CMenuManger(this);
	if (m_pMenuManger)
	{
		m_pMenuManger->InitMenuBar();
	}
}

void ShrikeDxf::InitTreeView()
{
	m_pTreeViewManger = new CTreeViewManger(this);
	if (m_pTreeViewManger)
	{
		//添加treeview
		m_pTreeViewManger->CreateTreeView();
	}
}

void ShrikeDxf::InitGraphicsView()
{
	m_pGraphicsView = new CGraphicsView(this);
}

void ShrikeDxf::InitStackedWidget()
{
	m_pStackedWidgetManger = new CStackedWidgetManger(this);
	if (m_pStackedWidgetManger)
	{
        m_pStackedWidgetManger->CreateStackedWidget();
	}
}

void ShrikeDxf::InitTableView()
{
	m_pTableViewManger = new CTableViewManger(this);
	if (m_pTableViewManger)
	{
		m_pTableViewManger->CreateTableView();
	}

}

void ShrikeDxf::ConnectSignalsAndSlots()
{
	QTimer::singleShot(0, this, [this]()
		{
			if (m_pDxfDataManger && m_pTreeViewManger)
			{
				connect(m_pDxfDataManger, &CDxfManger::RefreshTreeview,m_pTreeViewManger, &CTreeViewManger::handleRefreshTree);
				connect(m_pTreeViewManger, &CTreeViewManger::GetEntityData, m_pDxfDataManger, &CDxfManger::handleGetEntityData);
				connect(m_pDxfDataManger,&CDxfManger::ReturnEntityInfo,m_pTreeViewManger,&CTreeViewManger::handleReturnEntityInfo);
			}

			if (m_pDxfDataManger && m_pGraphicsView)
			{
				connect(m_pDxfDataManger,&CDxfManger::RefreshGraphicsview,m_pGraphicsView, &CGraphicsView::handleRefreshGraphicsview);
			}
	});


}

