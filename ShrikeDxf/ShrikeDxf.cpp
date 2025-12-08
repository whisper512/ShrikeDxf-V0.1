#include "ShrikeDxf.h"

ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	InitWindow();
	InitMenuBar();
	InitTreeView();
}

ShrikeDxf::~ShrikeDxf()
{
}

void ShrikeDxf::InitWindow()
{
	
}

void ShrikeDxf::InitMenuBar()
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

	ui.menuBar->addMenu(m_pMenuFile);
	ui.menuBar->addMenu(m_pMenuTool);
	ui.menuBar->addMenu(m_pMenuSetting);
	ui.menuBar->addMenu(m_pMenuHelp);
}

void ShrikeDxf::InitTreeView()
{
	QTreeView* m_pTreeView = new QTreeView(this);
	ui.VLayout_FileStructure->addWidget(m_pTreeView);
}



