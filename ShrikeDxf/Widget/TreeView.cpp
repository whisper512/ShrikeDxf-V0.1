#include "TreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
#include <QMessageBox>

TreeViewManager::TreeViewManager(QWidget* mainWnd) :
	m_mainWnd(mainWnd),
	m_contextMenu(nullptr),
	m_actionDelete(nullptr),
	m_actionCopy(nullptr),
	m_treeView(nullptr)
{
}

TreeViewManager::~TreeViewManager()
{
	delete m_treeView;
}

void TreeViewManager::createTreeView()
{
	m_treeView = new QTreeView(m_mainWnd);
	m_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if (m_mainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_mainWnd);
		pWnd->ui.verticalLayout_Tree->addWidget(m_treeView);
	}
	else
	{
		QMessageBox::information(m_treeView, "TreeView Error", "MainWnd is null");
		return;
	}

	//设置临时模型来显示列名
	QStandardItemModel* pHeaderModel = new QStandardItemModel(0, 2, m_treeView);
	pHeaderModel->setHeaderData(0, Qt::Horizontal, "LAYER");
	pHeaderModel->setHeaderData(1, Qt::Horizontal, "ENTITIES");


	m_treeView->setModel(pHeaderModel);
	QHeaderView* pHeader = m_treeView->header();
	pHeader->setSectionResizeMode(0, QHeaderView::Stretch);
	pHeader->setSectionResizeMode(1, QHeaderView::Stretch);

	//添加menu右键
	m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_treeView, &QTreeView::customContextMenuRequested, this, &TreeViewManager::handleShowContextMenu);
	//左键选择
	connect(m_treeView, &QTreeView::clicked, this, &TreeViewManager::handleOnItemClicked);
}


void TreeViewManager::handleShowContextMenu(const QPoint& pos)
{
	//initContextMenu();

	//m_contextMenu->exec(m_treeView->viewport()->mapToGlobal(pos));
}

void TreeViewManager::initContextMenu()
{
	if (m_treeView)
	{
		m_contextMenu = new QMenu(m_treeView);

		m_actionCopy = new QAction("Copy", this);
		m_contextMenu->addAction(m_actionCopy);
		connect(m_actionCopy, &QAction::triggered, this, &TreeViewManager::copyEntity);
		m_actionDelete = new QAction("Delete", this);
		m_contextMenu->addAction(m_actionDelete);
		connect(m_actionDelete,&QAction::triggered, this, &TreeViewManager::deleteEntity);
	}
}

void TreeViewManager::handleRefreshTreeviewAfterRead()
{
	if (!m_treeView || !m_treeView->model())
	{
		return;
	}

	QAbstractItemModel* model = m_treeView->model();

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

void TreeViewManager::deleteEntity()
{
	QModelIndex index = m_treeView->currentIndex();
	if (index.isValid())
	{
		QAbstractItemModel* model = m_treeView->model();
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
		emit signaldeleteEntityData(strLayer, strEntity);
	}
}

void TreeViewManager::copyEntity()
{
	emit signalcopyEntityData();
}


void TreeViewManager::handleReturnEntityInfo(QString strInfo)
{
    QMessageBox::information(m_treeView, "Entity Data", strInfo);
}

void TreeViewManager::handleOnItemClicked(const QModelIndex& index)
{
	if (!index.isValid()) return;

	QAbstractItemModel* model = m_treeView->model();
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



void TreeViewManager::handleRefreshTree(DxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_treeView->setModel(pModel);
		QHeaderView* pHeader = m_treeView->header();
		pHeader->setSectionResizeMode(0, QHeaderView::Stretch);
		pHeader->setSectionResizeMode(1, QHeaderView::Stretch);
		m_treeView->expandAll();
	}
}