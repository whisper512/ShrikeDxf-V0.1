#include "DxfTreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
#include <QMessageBox>

CTreeViewManger::CTreeViewManger(QWidget* pMainwnd):
	m_pMainwnd(pMainwnd),
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
	// 设置样式表，包括背景色和其他外观属性
	m_pTreeView->setStyleSheet(
		"QTreeView {"
		"	background-color: #d0d0d0;"  // 设置背景色为淡灰色
		"}"
	);


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
	connect(m_pTreeView,&QTreeView::customContextMenuRequested,this,&CTreeViewManger::ShowContextMenu);
}

void CTreeViewManger::ShowContextMenu(const QPoint& pos)
{
	QMenu* pContextMenu = new QMenu(m_pTreeView);
	QAction *ActionShowData = new QAction("Show Data", this);
    connect(ActionShowData, &QAction::triggered, this, &CTreeViewManger::ShowModelData);
	pContextMenu->addAction(ActionShowData);
	pContextMenu->exec(m_pTreeView->viewport()->mapToGlobal(pos));
}


void CTreeViewManger::ShowModelData()
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
		//获取详细信息
		emit GetEntityData(strLayer, strEntity);
	}
}

void CTreeViewManger::handleReturnEntityInfo(QString strInfo)
{
    QMessageBox::information(m_pTreeView, "Entity Data", strInfo);
}



void CTreeViewManger::handleRefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);
		m_pTreeView->expandAll();
	}
}