#include <QHeaderView>
#include <QColorDialog>

#include "LayerTableView.h"
#include "ShrikeDxf.h"

CLayerTableViewManger::CLayerTableViewManger(QWidget* pMainwnd):
    m_pMainWnd(pMainwnd),
    m_pTableView(nullptr)
{
}

CLayerTableViewManger::~CLayerTableViewManger()
{
}

void CLayerTableViewManger::CreateTableView()
{
    m_pTableView = new QTableView(m_pMainWnd);
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_pTableView);
		
	}
    InitTableView();
	
}

void CLayerTableViewManger::InitTableView()
{
	m_pTableView->setStyleSheet(
		"QTableView {"
		"    background-color: #d0d0d0;"
		"}"
		"QTableView::item {"
		"    text-align: center;"
		"    padding: 5px;"
		"}"
		"QHeaderView::section {"
		"    background-color: #e0e0e0;"
		"    padding: 4px;"
		"    border: 1px solid #e0e0e0;"
		"}"
	);

	QStandardItemModel* pHeaderModel = new QStandardItemModel(0,3, m_pTableView);
	pHeaderModel->setHorizontalHeaderLabels({"NO","NAME","COLOR"});
	m_pTableView->setModel(pHeaderModel);
	m_pTableView->verticalHeader()->hide();
	m_pTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	m_pTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_pTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

	connect(m_pTableView, &QTableView::clicked, this, &CLayerTableViewManger::handleTableViewClicked);
}

void CLayerTableViewManger::handleTableViewClicked(const QModelIndex& index)
{
	if (index.column() == 2)
	{
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
	emit signalLayerModelChanged();
}


void CLayerTableViewManger::handleRefreshLayerTableview(CDxfLayerTableviewModel* pModel)
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
	}
}
