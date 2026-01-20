#include "DxfTreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
#include <QMessageBox>

CTreeViewManger::CTreeViewManger(QWidget* pMainwnd) :
	m_pMainwnd(pMainwnd),
	m_pContextMenu(nullptr),
	m_pActionDelete(nullptr),
	m_pActionCopy(nullptr),
	m_pTreeView(nullptr)
{
}

CTreeViewManger::~CTreeViewManger()
{
	delete m_pTreeView;
}

void CTreeViewManger::CreateTreeView()
{
	m_pTreeView = new QTreeView(m_pMainwnd);
	m_pTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if (m_pMainwnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainwnd);
		pWnd->ui.verticalLayout_FileStructure->addWidget(m_pTreeView);
	}
	else
	{
		QMessageBox::information(m_pTreeView, "TreeView Error", "MainWnd is null");
		return;
	}
	// 设置样式表，包括背景色和其他外观属性
	m_pTreeView->setStyleSheet(
		"QTreeView {"
		"	background-color: #d0d0d0;"  // 设置背景色为淡灰色
		"}"
		"QTreeView::item {"
		"   height: 24px;"  // 设置行高为32像素，与图标尺寸匹配
		"}"
		"QHeaderView::section {"
		"    background-color: #e0e0e0;"  // 设置表头颜色
		"    padding: 4px;"
		"    border: 1px solid #e0e0e0;"
		"}"
	);

	//m_pTreeView->setIconSize(QSize(16, 16));

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
	connect(m_pTreeView, &QTreeView::customContextMenuRequested, this, &CTreeViewManger::handleShowContextMenu);
	//左键选择
	connect(m_pTreeView, &QTreeView::clicked, this, &CTreeViewManger::handleOnItemClicked);
}


void CTreeViewManger::handleShowContextMenu(const QPoint& pos)
{
	InitContextMenu();

	m_pContextMenu->exec(m_pTreeView->viewport()->mapToGlobal(pos));
}

void CTreeViewManger::InitContextMenu()
{
	if (m_pTreeView)
	{
		m_pContextMenu = new QMenu(m_pTreeView);

		m_pActionCopy = new QAction("Copy", this);
		m_pContextMenu->addAction(m_pActionCopy);
		connect(m_pActionCopy, &QAction::triggered, this, &CTreeViewManger::CopyEntity);
		m_pActionDelete = new QAction("Delete", this);
		m_pContextMenu->addAction(m_pActionDelete);
		connect(m_pActionDelete,&QAction::triggered, this, &CTreeViewManger::DeleteEntity);
	}
}

void CTreeViewManger::handleRefreshTreeviewAfterRead()
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

	emit signalSaveSelectedEntity(strLayer, strEntity);

	
}

void CTreeViewManger::DeleteEntity()
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

void CTreeViewManger::CopyEntity()
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

		}
		QString strEntity = model->data(index.sibling(index.row(), 1), Qt::DisplayRole).toString();
		emit signalCopyEntityData(strLayer, strEntity);
	}
}


void CTreeViewManger::handleReturnEntityInfo(QString strInfo)
{
    QMessageBox::information(m_pTreeView, "Entity Data", strInfo);
}

void CTreeViewManger::handleOnItemClicked(const QModelIndex& index)
{
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
		emit signalSaveSelectedEntity(strLayer, strEntity);
	}
}



void CTreeViewManger::handleRefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);
		//m_pTreeView->setIconSize(QSize(16, 16));
		QHeaderView* pHeader = m_pTreeView->header();
		pHeader->setSectionResizeMode(0, QHeaderView::Stretch);
		pHeader->setSectionResizeMode(1, QHeaderView::Stretch);
		m_pTreeView->expandAll();
	}
}