#include "TopMenu.h"
#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#include "ShrikeDxf.h"

MenuManager::MenuManager(QWidget* parent) : 
	QWidget(parent),
	m_menuFile(nullptr),
	m_menuHelp(nullptr),
	m_menuSetting(nullptr),
	m_menuView(nullptr),
	m_actionOpen(nullptr),
	m_actionSave(nullptr),
	m_actionClose(nullptr),
	m_actionAbout(nullptr)
{
	m_parent = this->parentWidget();
}

MenuManager::~MenuManager()
{
	delete m_menuFile;
	delete m_menuHelp;
	delete m_menuView;
	delete m_actionOpen;
	delete m_actionSave;
	delete m_actionClose;
	delete m_actionAbout;
}

void MenuManager::initMenuBar()
{
	initMenu();
	initAction();
	addToBar();
	connectSlot();
}

void MenuManager::initMenu()
{
	m_menuFile = new QMenu("File", this);
	m_menuSetting = new QMenu("Setting", this);
	m_menuView = new QMenu("View", this);
}

void MenuManager::initAction()
{
	if (m_menuFile)
	{
		if (m_menuFile->actions().isEmpty())
		{
			m_actionNew = new QAction("New", this);
			m_actionNew->setShortcut(QKeySequence::New);
			m_menuFile->addAction(m_actionNew);
			m_actionOpen = new QAction("Open", this);
			m_actionOpen->setShortcut(QKeySequence::Open);
			m_menuFile->addAction(m_actionOpen);
			m_actionSave = new QAction("Save", this);
			m_actionSave->setShortcut(QKeySequence::Save);
			m_menuFile->addAction(m_actionSave);
            m_actionSaveAs = new QAction("Save As", this);
            m_actionSaveAs->setShortcut(QKeySequence::SaveAs);
            m_menuFile->addAction(m_actionSaveAs);
			m_actionClose = new QAction("Close", this);
			m_actionClose->setShortcut(QKeySequence::Close);
			m_menuFile->addAction(m_actionClose);
		}
	}

	if (m_menuView)
	{
		if (m_menuView->actions().isEmpty())
		{
			
		}
	}

	if (m_menuHelp)
	{
		if (m_menuHelp->actions().isEmpty())
		{
            m_actionAbout = new QAction("About", this);
			m_menuHelp->addAction(m_actionAbout);
		}
	}
}

void MenuManager::addToBar()
{
	if (m_parent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);
		if(pShrikeDxf)
		{
			pShrikeDxf->menuBar()->addMenu(m_menuFile);
			pShrikeDxf->menuBar()->addMenu(m_menuSetting);
			pShrikeDxf->menuBar()->addMenu(m_menuView);
		}
	}
}

void MenuManager::connectSlot()
{
	if (m_actionOpen)
	{
		connect(m_actionOpen, &QAction::triggered,this, &MenuManager::onOpen);
	}
	if (m_actionSave)
	{
		connect(m_actionSave, &QAction::triggered, this, &MenuManager::onSave);
	}
	if (m_actionClose)
	{
		connect(m_actionClose, &QAction::triggered, this, &MenuManager::onClose);
	}
	if (m_actionSaveAs)
	{
        connect(m_actionSaveAs, &QAction::triggered, this, &MenuManager::onSaveAs);
	}
	if(m_actionNew)
	{
        connect(m_actionNew, &QAction::triggered, this, &MenuManager::onNew);
	}
}

void MenuManager::onOpen()
{
	if (m_parent)
	{
		QString filePath = QFileDialog::getOpenFileName(
			this,
			"Select DXF File",
			QDir::homePath(),
			"DXF File (*.dxf);;All Files (*.*)"
		);
		if (!filePath.isEmpty())
		{
			ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);
			pShrikeDxf->m_dataManager->m_dxfPath = filePath;
			pShrikeDxf->m_dxfDataManager->loadDxfFile(filePath);
		}
	}
}

void MenuManager::onSave()
{
	if (m_parent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);
		QString filePath = pShrikeDxf->m_dataManager->m_dxfPath;
		pShrikeDxf->m_dxfDataManager->saveDxfFile(filePath);
	}
}

void MenuManager::onClose()
{
	if (m_parent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);
        pShrikeDxf->m_dxfDataManager->closeDxfFile();
	}
}

void MenuManager::onNew()
{
	if (m_parent)
	{
        ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);
        pShrikeDxf->m_dxfDataManager->newDxfFile();
	}
}

void MenuManager::onSaveAs()
{
	if (m_parent)
	{
		ShrikeDxf* pShrikeDxf = dynamic_cast<ShrikeDxf*>(m_parent);

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
