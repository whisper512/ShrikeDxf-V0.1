#include <QHeaderView>
#include <QColorDialog>

#include "LayerTableView.h"
#include "ShrikeDxf.h"

LayerTableViewManager::LayerTableViewManager(QWidget* mainWnd):
    m_mainWnd(mainWnd),
    m_tableView(nullptr)
{
}

LayerTableViewManager::~LayerTableViewManager()
{
}

void LayerTableViewManager::createTableView()
{
    m_tableView = new QTableView(m_mainWnd);
	if (m_mainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_mainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_tableView);
		
	}
    initTableView();
	
}

void LayerTableViewManager::initTableView()
{
	QStandardItemModel* pHeaderModel = new QStandardItemModel(0,3, m_tableView);
	pHeaderModel->setHorizontalHeaderLabels({"NO","NAME","COLOR"});
	m_tableView->setModel(pHeaderModel);
	m_tableView->verticalHeader()->hide();
	m_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	m_tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	connect(m_tableView, &QTableView::clicked, this, &LayerTableViewManager::handleTableViewClicked);
}

void LayerTableViewManager::handleTableViewClicked(const QModelIndex& index)
{
	if (index.column() == 2)
	{
		//直接修改模型
		QStandardItemModel* pModel = qobject_cast<QStandardItemModel*>(m_tableView->model());
		if (pModel)
		{
			QColor color = QColorDialog::getColor(Qt::white, m_tableView, "");
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


void LayerTableViewManager::handleRefreshLayerTableview(DxfLayerTableviewModel* pModel)
{
	if (pModel)
	{
		m_tableView->setModel(pModel);
		for (int col = 0; col < pModel->columnCount(); col++)
		{
			m_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
		}

		for (int col = 0; col < pModel->columnCount(); col++)
		{
			m_tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
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
