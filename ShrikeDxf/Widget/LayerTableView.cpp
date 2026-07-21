#include <QHeaderView>
#include <QColorDialog>

#include "LayerTableView.h"
#include "ShrikeDxf.h"

LayerTableViewManager::LayerTableViewManager(QWidget* pMainwnd):
    m_pMainWnd(pMainwnd),
    m_pTableView(nullptr)
{
}

LayerTableViewManager::~LayerTableViewManager()
{
}

void LayerTableViewManager::CreateTableView()
{
    m_pTableView = new QTableView(m_pMainWnd);
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_pTableView);
		
	}
    InitTableView();
	
}

void LayerTableViewManager::InitTableView()
{
	QStandardItemModel* pHeaderModel = new QStandardItemModel(0,3, m_pTableView);
	pHeaderModel->setHorizontalHeaderLabels({"NO","NAME","COLOR"});
	m_pTableView->setModel(pHeaderModel);
	m_pTableView->verticalHeader()->hide();
	m_pTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	m_pTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_pTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	connect(m_pTableView, &QTableView::clicked, this, &LayerTableViewManager::handleTableViewClicked);
}

void LayerTableViewManager::handleTableViewClicked(const QModelIndex& index)
{
	if (index.column() == 2)
	{
		//直接修改模型
		QStandardItemModel* pModel = qobject_cast<QStandardItemModel*>(m_pTableView->model());
		if (pModel)
		{
			QColor color = QColorDialog::getColor(Qt::white, m_pTableView, "");
			if (color.isValid())
			{
				pModel->setData(index, color.name(), Qt::EditRole);
				pModel->setData(index, color, Qt::BackgroundRole);
			}
		}
	}

	//通知模型已经修改,刷新
	emit signalLayerModelChanged();
}


void LayerTableViewManager::handleRefreshLayerTableview(CDxfLayerTableviewModel* pModel)
{
	if (pModel)
	{
		m_pTableView->setModel(pModel);
		for (int col = 0; col < pModel->columnCount(); col++)
		{
			m_pTableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
		}

		for (int col = 0; col < pModel->columnCount(); col++)
		{
			m_pTableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
		}
		// 获取第一行的图层名称并发送信号
		if (pModel->rowCount() > 0)
		{
			QModelIndex firstLayerIndex = pModel->index(0, 1);
			QString strCurLayer = pModel->data(firstLayerIndex).toString();
			//加载后默认工作图层为第一个图层
			emit signalChangeCurrentLayer(strCurLayer);
		}
	}
}
