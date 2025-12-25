
#include "TreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
#include <QMessageBox>

CTreeView::CTreeView(QWidget* pMainwnd):
	m_pMainwnd(pMainwnd),
	m_pTreeView(nullptr)
{
}

CTreeView::~CTreeView()
{
	delete m_pTreeView;
}

void CTreeView::CreateTreeView()
{
	m_pTreeView = new QTreeView(m_pMainwnd);
	
	if (m_pMainwnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainwnd);
		pWnd->ui.verticalLayout_FileStructure->addWidget(m_pTreeView);
	}

	m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_pTreeView,&QTreeView::customContextMenuRequested,this,&CTreeView::ShowContextMenu);
}

void CTreeView::ShowContextMenu(const QPoint& pos)
{
	QMenu* pContextMenu = new QMenu(m_pTreeView);
	QAction *ActionShowData = new QAction("Show Data", this);
    connect(ActionShowData, &QAction::triggered, this, &CTreeView::ShowModelData);
	pContextMenu->addAction(ActionShowData);
	pContextMenu->exec(m_pTreeView->viewport()->mapToGlobal(pos));
}


void CTreeView::ShowModelData()
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

void CTreeView::handleReturnEntityInfo(QString strInfo)
{
    QMessageBox::information(m_pTreeView, "Entity Data", strInfo);
}



void CTreeView::RefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);

		QHeaderView* pHeader = m_pTreeView->header();
		int iWidth = m_pTreeView->width();
		pHeader->resizeSection(0, iWidth * 0.5);
		pHeader->resizeSection(1, iWidth * 0.5);
		m_pTreeView->expandAll();
	}
}