#include "TopMenu.h"
#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "ShrikeDxf.h"

MenuManager::MenuManager(QWidget* parent) : 
	QWidget(parent),
	m_pMenuFile(nullptr),
	m_pMenuHelp(nullptr),
	m_pMenuSetting(nullptr),
	m_pMenuView(nullptr),
	m_pActionOpen(nullptr),
	m_pActionSave(nullptr),
	m_pActionClose(nullptr),
	m_pActionAbout(nullptr)
{
	m_pParent = this->parentWidget();
}

MenuManager::~MenuManager()
{
	delete m_pMenuFile;
	delete m_pMenuHelp;
	delete m_pMenuView;
	delete m_pActionOpen;
	delete m_pActionSave;
	delete m_pActionClose;
	delete m_pActionAbout;
}

void MenuManager::initMenuBar()
{
	initMenu();
	InitAction();
	AddToBar();
	ConnectSolt();
}

void MenuManager::initMenu()
{
	m_pMenuFile = new QMenu("File", this);
	m_pMenuSetting = new QMenu("Setting", this);
	m_pMenuView = new QMenu("View", this);
}

void MenuManager::InitAction()
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

void MenuManager::AddToBar()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		if(pShrikeDxf)
		{
			pShrikeDxf->menuBar()->addMenu(m_pMenuFile);
			pShrikeDxf->menuBar()->addMenu(m_pMenuSetting);
			pShrikeDxf->menuBar()->addMenu(m_pMenuView);
		}
	}
}

void MenuManager::ConnectSolt()
{
	if (m_pActionOpen)
	{
		connect(m_pActionOpen, &QAction::triggered,this, &MenuManager::OnOpen);
	}
	if (m_pActionSave)
	{
		connect(m_pActionSave, &QAction::triggered, this, &MenuManager::OnSave);
	}
	if (m_pActionClose)
	{
		connect(m_pActionClose, &QAction::triggered, this, &MenuManager::OnClose);
	}
	if (m_pActionSaveAs)
	{
        connect(m_pActionSaveAs, &QAction::triggered, this, &MenuManager::OnSaveAs);
	}
	if(m_pActionNew)
	{
        connect(m_pActionNew, &QAction::triggered, this, &MenuManager::OnNew);
	}
}

void MenuManager::OnOpen()
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
			pShrikeDxf->m_dataManager->m_dxfPath = filePath;
			pShrikeDxf->m_dxfDataManager->loadDxfFile(filePath);
		}
	}
}

void MenuManager::OnSave()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
		QString filePath = pShrikeDxf->m_dataManager->m_dxfPath;
		pShrikeDxf->m_dxfDataManager->saveDxfFile(filePath);
	}
}

void MenuManager::OnClose()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
        pShrikeDxf->m_dxfDataManager->closeDxfFile();
	}
}

void MenuManager::OnNew()
{
	if (m_pParent)
	{
        ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);
        pShrikeDxf->m_dxfDataManager->newDxfFile();
	}
}

void MenuManager::OnSaveAs()
{
	if (m_pParent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_pParent);

		QString filePath = QFileDialog::getSaveFileName(
			this,
			"Save DXF File",
			QDir::homePath(),
			"DXF File (*.dxf);;All Files (*.*)"
		);

		if (!filePath.isEmpty())
		{
			if (!filePath.endsWith(".dxf", Qt::CaseInsensitive))
			{
				filePath += ".dxf";
			}

			bool success = pShrikeDxf->m_dxfDataManager->saveDxfFile(filePath);

			// 如果保存成功，更新当前文件路径
			if (success)
			{
				pShrikeDxf->m_dataManager->m_dxfPath = filePath;
			}
			else
			{
				// 显示保存失败的消息
				QMessageBox::warning(this, "Save File", "Failed to save DXF file.");
			}
		}
	}
}
