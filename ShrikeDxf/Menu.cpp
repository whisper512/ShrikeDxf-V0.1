#include "Menu.h"
#include <QWidget>
#include <QFileDialog>
#include <QDebug>

#include "ShrikeDxf.h"

CMenu::CMenu(QWidget* parent) : 
	QWidget(parent),
	m_pMenuFile(nullptr),
	m_pMenuHelp(nullptr),
	m_pMenuTool(nullptr),
	m_pMenuSetting(nullptr),
	m_pMenuView(nullptr),
	m_pActionOpen(nullptr),
	m_pActionSave(nullptr),
	m_pActionClose(nullptr),
	m_pActionAbout(nullptr),
	m_pActionLightTheme(nullptr),
	m_pActionDarkTheme(nullptr)
{
	m_pParent = this->parentWidget();
}

CMenu::~CMenu()
{
	delete m_pMenuFile;
	delete m_pMenuHelp;
	delete m_pMenuTool;
	delete m_pMenuSetting;
	delete m_pMenuView;
	delete m_pActionOpen;
	delete m_pActionSave;
	delete m_pActionClose;
	delete m_pActionAbout;
	delete m_pActionLightTheme;
	delete m_pActionDarkTheme;
}

void CMenu::InitMenuBar()
{
	InitMenu();
	InitAction();
	AddToBar();
	ConnectSolt();
}

void CMenu::InitMenu()
{
	m_pMenuFile = new QMenu("File", this);
	m_pMenuTool = new QMenu("Tool", this);
	m_pMenuSetting = new QMenu("Setting", this);
	m_pMenuHelp = new QMenu("Help", this);
	m_pMenuView = new QMenu("View", this);
}

void CMenu::InitAction()
{
	if (m_pMenuFile)
	{
		if (m_pMenuFile->actions().isEmpty())
		{
			m_pActionOpen = new QAction("Open", this);
			m_pActionOpen->setShortcut(QKeySequence::Open);
			m_pMenuFile->addAction(m_pActionOpen);
			m_pActionSave = new QAction("Save", this);
			m_pActionSave->setShortcut(QKeySequence::Save);
			m_pMenuFile->addAction(m_pActionSave);
			m_pActionClose = new QAction("Close", this);
			m_pActionClose->setShortcut(QKeySequence::Close);
			m_pMenuFile->addAction(m_pActionClose);
		}
	}

	if (m_pMenuView)
	{
		if (m_pMenuView->actions().isEmpty())
		{
			m_pActionLightTheme = new QAction("Light Theme", this);
			m_pMenuView->addAction(m_pActionLightTheme);
			m_pActionDarkTheme = new QAction("Dark Theme", this);
			m_pMenuView->addAction(m_pActionDarkTheme);
		}
	}

	if (m_pMenuHelp)
	{
		if (m_pMenuHelp->actions().isEmpty())
		{
            m_pActionAbout = new QAction("About", this);
			m_pMenuHelp->addAction(m_pActionAbout);
		}
	}
}

void CMenu::AddToBar()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		if(pShrikeDxf)
		{
			pShrikeDxf->menuBar()->addMenu(m_pMenuFile);
            pShrikeDxf->menuBar()->addMenu(m_pMenuTool);
			pShrikeDxf->menuBar()->addMenu(m_pMenuSetting);
			pShrikeDxf->menuBar()->addMenu(m_pMenuView);
			pShrikeDxf->menuBar()->addMenu(m_pMenuHelp);
		}
	}
}

void CMenu::ConnectSolt()
{
	if (m_pActionOpen)
	{
		connect(m_pActionOpen, &QAction::triggered,this, &CMenu::OnOpen);
	}
	if (m_pActionSave)
	{
		connect(m_pActionSave, &QAction::triggered, this, &CMenu::OnSave);
	}
	if (m_pActionClose)
	{
		connect(m_pActionClose, &QAction::triggered, this, &CMenu::OnClose);
	}

	if (m_pActionLightTheme)
	{
        connect(m_pActionLightTheme, &QAction::triggered, this, &CMenu::OnLightTheme);
	}
	if (m_pActionDarkTheme)
	{
		connect(m_pActionDarkTheme, &QAction::triggered, this, &CMenu::OnDarkTheme);
	}
}

void CMenu::OnOpen()
{
	if (m_pParent)
	{
		QString filePath = QFileDialog::getOpenFileName(
			this,
			"Select DXF File",
			QDir::homePath(),
			"DXF File (*.dxf);;All Files (*.*)"
		);
		if (!filePath.isEmpty())
		{
			ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
			pShrikeDxf->m_pDataManager->m_strDxfPath = filePath;
			//pShrikeDxf->m_pDxfDataManger->SeterDxfFilePath(filePath);
		}
	}
}

void CMenu::OnSave()
{
}

void CMenu::OnClose()
{
}

void CMenu::OnLightTheme()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		qApp->setPalette(*(pShrikeDxf->m_pDataManager->getPaletteLight(eThremColor_Light)));
		// 为菜单添加样式表
	}
}

void CMenu::OnDarkTheme()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		qApp->setPalette(*(pShrikeDxf->m_pDataManager->getPaletteLight(eThremColor_Dark)));
	}
}
