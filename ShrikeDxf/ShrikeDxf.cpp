#include "ShrikeDxf.h"


ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent),
	m_pMenu(nullptr),
	m_pTreeView(nullptr)
{
    ui.setupUi(this);
	setWindowIcon(QIcon(":/ShrikeDxf/res/Main.png"));

	InitWindowComponents();
	InitDataManagers();
}

ShrikeDxf::~ShrikeDxf()
{
	delete m_pMenu;
    delete m_pTreeView;
}

void ShrikeDxf::InitWindowComponents()
{
	//初始化组件需要在确定ui界面加载完毕后进行,在进入消息循环后定时器开启执行初始化
	QTimer::singleShot(0, this, [this]()
	{
		InitMenuBar();
		InitTreeView();
	});
}

void ShrikeDxf::InitDataManagers()
{
	QTimer::singleShot(0, this, [this]()
	{
		m_pDataManager = new CCommonDataManager();
		m_pDxfDataManger = new CDxfDataManger();
	});
}


void ShrikeDxf::InitMenuBar()
{
	m_pMenu = new CMenu(this);
	if (m_pMenu)
	{
		m_pMenu->InitMenuBar();
	}
}

void ShrikeDxf::InitTreeView()
{

}

