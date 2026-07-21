#include "TreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
#include <QMessageBox>

TreeViewManager::TreeViewManager(QWidget* pMainwnd) :
	m_pMainwnd(pMainwnd),
	m_pContextMenu(nullptr),
	m_pActionDelete(nullptr),
	m_pActionCopy(nullptr),
	m_pTreeView(nullptr)
{
}

TreeViewManager::~TreeViewManager()
{
	delete m_pTreeView;
}

void TreeViewManager::CreateTreeView()
{
	m_pTreeView = new QTreeView(m_pMainwnd);
	m_pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if (m_pMainwnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainwnd);
		pWnd->ui.verticalLayout_Tree->addWidget(m_pTreeView);
	}
	else
	{
		QMessageBox::information(m_pTreeView, "TreeView Error", "MainWnd is null");
		return;
	}

	//设置临时模型来显示列名
	QStandardItemModel* pHeaderModel = new QStandardItemModel(0, 2, m_pTreeView);
	pHeaderModel->setHeaderData(0, Qt::Horizontal, "LAYER");
	pHeaderModel->setHeaderData(1, Qt::Horizontal, "ENTITIES");


	m_pTreeView->setModel(pHeaderModel);
	QHeaderView* pHeader = m_pTreeView->header();
	pHeader->setSectionResizeMode(0, QHeaderView::Stretch);
	pHeader->setSectionResizeMode(1, QHeaderView::Stretch);

	//添加menu右键
	m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pTreeView, &QTreeView::customContextMenuRequested, this, &TreeViewManager::handleShowContextMenu);
	//左键选择
	connect(m_pTreeView, &QTreeView::clicked, this, &TreeViewManager::handleOnItemClicked);
}


void TreeViewManager::handleShowContextMenu(const QPoint& pos)
{
	//InitContextMenu();

	//m_pContextMenu->exec(m_pTreeView->viewport()->mapToGlobal(pos));
}

void TreeViewManager::InitContextMenu()
{
	if (m_pTreeView)
	{
		m_pContextMenu = new QMenu(m_pTreeView);

		m_pActionCopy = new QAction("Copy", this);
		m_pContextMenu->addAction(m_pActionCopy);
		connect(m_pActionCopy, &QAction::triggered, this, &TreeViewManager::CopyEntity);
		m_pActionDelete = new QAction("Delete", this);
		m_pContextMenu->addAction(m_pActionDelete);
		connect(m_pActionDelete,&QAction::triggered, this, &TreeViewManager::DeleteEntity);
	}
}

void TreeViewManager::handleRefreshTreeviewAfterRead()
{
	if (!m_pTreeView || !m_pTreeView->model())
	{
		return;
	}

	QAbstractItemModel* model = m_pTreeView->model();

	QModelIndex firstLayerIndex = model->index(0, 0);

	if (!firstLayerIndex.isValid())
	{
		return; 
	}
	QModelIndex firstEntityIndex = model->index(0, 0, firstLayerIndex);

	if (!firstEntityIndex.isValid())
	{
		return; 
	}
	QString strLayer = model->data(firstLayerIndex, Qt::DisplayRole).toString();
	QString strEntity = model->data(firstEntityIndex.sibling(firstEntityIndex.row(), 1), Qt::DisplayRole).toString();	
}

void TreeViewManager::DeleteEntity()
{
	QModelIndex index = m_pTreeView->currentIndex();
	if (index.isValid())
	{
		QAbstractItemModel* model = m_pTreeView->model();
		QModelIndex ParentIndex = index.parent();
		QString strLayer;

		if (ParentIndex.isValid())
		{
			strLayer = model->data(ParentIndex, Qt::DisplayRole).toString();
		}
		else
		{
			//选择到了图层本身
		}
		QString strEntity = model->data(index.sibling(index.row(), 1), Qt::DisplayRole).toString();
		emit signalDeleteEntityData(strLayer, strEntity);
	}
}

void TreeViewManager::CopyEntity()
{
	emit signalCopyEntityData();
}


void TreeViewManager::handleReturnEntityInfo(QString strInfo)
{
    QMessageBox::information(m_pTreeView, "Entity Data", strInfo);
}

void TreeViewManager::handleOnItemClicked(const QModelIndex& index)
{
	if (!index.isValid()) return;

	QAbstractItemModel* model = m_pTreeView->model();
	QModelIndex parentIndex = index.parent();

	if (parentIndex.isValid())
	{
		QString strLayer = model->data(parentIndex, Qt::DisplayRole).toString();
		QModelIndex entityIndexIdx = index.sibling(index.row(), 1);
		int entityIndex = model->data(entityIndexIdx, Qt::UserRole).toInt();

		emit signalEntitySelected(strLayer, entityIndex);
	}
	else
	{
		if (index.column() == 0)
		{
			QString strLayer = model->data(index, Qt::DisplayRole).toString();
			emit signalEntitySelected(strLayer, -1);
		}
	}
}



void TreeViewManager::handleRefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);
		QHeaderView* pHeader = m_pTreeView->header();
		pHeader->setSectionResizeMode(0, QHeaderView::Stretch);
		pHeader->setSectionResizeMode(1, QHeaderView::Stretch);
		m_pTreeView->expandAll();
	}
}