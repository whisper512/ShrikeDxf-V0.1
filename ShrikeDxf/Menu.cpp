#include "Menu.h"
#include <QWidget>
#include "ShrikeDxf.h"

CMenu::CMenu(QWidget* parent) : 
	QWidget(parent),
	m_pMenuFile(nullptr),
	m_pMenuHelp(nullptr),
	m_pMenuTool(nullptr),
	m_pMenuSetting(nullptr),
	m_pActionOpen(nullptr),
	m_pActionSave(nullptr),
	m_pActionClose(nullptr),
	m_pActionAbout(nullptr)
{
	m_pParent = this->parentWidget();
	InitMenuBar();
}

CMenu::~CMenu()
{
	delete m_pMenuFile;
	delete m_pMenuHelp;
	delete m_pMenuTool;
	delete m_pMenuSetting;
	delete m_pActionOpen;
	delete m_pActionSave;
	delete m_pActionClose;
	delete m_pActionAbout;
}

void CMenu::InitMenuBar()
{
	m_pMenuFile = new QMenu("File", this);
	m_pActionOpen = new QAction("Open", this);
	m_pActionOpen->setShortcut(QKeySequence::Open);
	m_pMenuFile->addAction(m_pActionOpen);
	m_pActionSave = new QAction("Save", this);
	m_pActionSave->setShortcut(QKeySequence::Save);
	m_pMenuFile->addAction(m_pActionSave);
	m_pActionClose = new QAction("Close", this);
	m_pActionClose->setShortcut(QKeySequence::Close);
	m_pMenuFile->addAction(m_pActionClose);
	
	m_pMenuHelp = new QMenu("Help", this);
	m_pActionAbout = new QAction("About", this);
	m_pMenuHelp->addAction(m_pActionAbout);

	m_pMenuTool = new QMenu("Tool", this);

	m_pMenuSetting = new QMenu("Setting", this);



	
	m_pParent = this->parentWidget();
	if (m_pParent)
	{
		ShrikeDxf* mainWindow = qobject_cast<ShrikeDxf*>(m_pParent);
		if (mainWindow)
		{
			mainWindow->menuBar()->addMenu(m_pMenuFile);
			mainWindow->menuBar()->addMenu(m_pMenuTool);
			mainWindow->menuBar()->addMenu(m_pMenuSetting);
			mainWindow->menuBar()->addMenu(m_pMenuHelp);
		}
	}
}

