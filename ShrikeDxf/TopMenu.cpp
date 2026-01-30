#include "TopMenu.h"
#include <QWidget>
#include <QFileDialog>
#include <QDebug>

#include "ShrikeDxf.h"

CMenuManger::CMenuManger(QWidget* parent) : 
	QWidget(parent),
	m_pMenuFile(nullptr),
	m_pMenuHelp(nullptr),
	m_pMenuTool(nullptr),
	m_pMenuSetting(nullptr),
	m_pMenuView(nullptr),
	m_pActionOpen(nullptr),
	m_pActionSave(nullptr),
	m_pActionClose(nullptr),
	m_pActionAbout(nullptr)
{
	m_pParent = this->parentWidget();
}

CMenuManger::~CMenuManger()
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
}

void CMenuManger::InitMenuBar()
{
	InitMenu();
	InitAction();
	AddToBar();
	ConnectSolt();
}

void CMenuManger::InitMenu()
{
	m_pMenuFile = new QMenu("File", this);
	m_pMenuTool = new QMenu("Tool", this);
	m_pMenuSetting = new QMenu("Setting", this);
	m_pMenuHelp = new QMenu("Help", this);
	m_pMenuView = new QMenu("View", this);
}

void CMenuManger::InitAction()
{
	if (m_pMenuFile)
	{
		if (m_pMenuFile->actions().isEmpty())
		{
			m_pActionNew = new QAction("New", this);
			m_pActionNew->setShortcut(QKeySequence::New);
			m_pMenuFile->addAction(m_pActionNew);
			m_pActionOpen = new QAction("Open", this);
			m_pActionOpen->setShortcut(QKeySequence::Open);
			m_pMenuFile->addAction(m_pActionOpen);
			m_pActionSave = new QAction("Save", this);
			m_pActionSave->setShortcut(QKeySequence::Save);
			m_pMenuFile->addAction(m_pActionSave);
            m_pActionSaveAs = new QAction("Save As", this);
            m_pActionSaveAs->setShortcut(QKeySequence::SaveAs);
            m_pMenuFile->addAction(m_pActionSaveAs);
			m_pActionClose = new QAction("Close", this);
			m_pActionClose->setShortcut(QKeySequence::Close);
			m_pMenuFile->addAction(m_pActionClose);
		}
	}

	if (m_pMenuView)
	{
		if (m_pMenuView->actions().isEmpty())
		{
			
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

void CMenuManger::AddToBar()
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

void CMenuManger::ConnectSolt()
{
	if (m_pActionOpen)
	{
		connect(m_pActionOpen, &QAction::triggered,this, &CMenuManger::OnOpen);
	}
	if (m_pActionSave)
	{
		connect(m_pActionSave, &QAction::triggered, this, &CMenuManger::OnSave);
	}
	if (m_pActionClose)
	{
		connect(m_pActionClose, &QAction::triggered, this, &CMenuManger::OnClose);
	}
	if (m_pActionSaveAs)
	{
        connect(m_pActionSaveAs, &QAction::triggered, this, &CMenuManger::OnSaveAs);
	}
	if(m_pActionNew)
	{
        connect(m_pActionNew, &QAction::triggered, this, &CMenuManger::OnNew);
	}
}

void CMenuManger::OnOpen()
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
			pShrikeDxf->m_pDxfDataManger->LoadDxfFile(filePath);
		}
	}
}

void CMenuManger::OnSave()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		QString filePath = pShrikeDxf->m_pDataManager->m_strDxfPath;
		pShrikeDxf->m_pDxfDataManger->SaveDxfFile(filePath);
	}

}

void CMenuManger::OnClose()
{

}

void CMenuManger::OnNew()
{
}

void CMenuManger::OnSaveAs()
{
}
