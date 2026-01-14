#include "PolylineAttributeWidget.h"

CPolylineAttributeWidget::CPolylineAttributeWidget(QWidget* parent)
	: QWidget(parent),
	m_polyline()
{
	ui.setupUi(this);
	ui.spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);


	connect(ui.tableWidget, &QTableWidget::itemChanged, this, &CPolylineAttributeWidget::OnTableValueChanged);
}

CPolylineAttributeWidget::~CPolylineAttributeWidget()
{
}

void CPolylineAttributeWidget::RefreshTable()
{
	ui.tableWidget->setRowCount(m_polyline.numVertices);
	ui.tableWidget->setColumnCount(2);

	QStringList header;
	header << "X" << "Y";
	ui.tableWidget->setHorizontalHeaderLabels(header);
	for (int i = 0; i < m_polyline.numVertices; i++)
	{
		QTableWidgetItem* itemX = new QTableWidgetItem(QString::number(m_polyline.vecVertices[i].x));
		ui.tableWidget->setItem(i, 0, itemX);
		QTableWidgetItem* itemY = new QTableWidgetItem(QString::number(m_polyline.vecVertices[i].y));
		ui.tableWidget->setItem(i, 1, itemY);
	}
}

void CPolylineAttributeWidget::OnTableValueChanged()
{
	m_polyline.vecVertices.clear();
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
		double x = itemX ? itemX->text().toDouble() : 0.0;

		QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
		double y = itemY ? itemY->text().toDouble() : 0.0;

		// 创建顶点并添加到向量中
		Point vertex;
		vertex.x = x;
		vertex.y = y;
		m_polyline.vecVertices.push_back(vertex);
	}
	emit signalPolylineAttributeChanged(m_polyline);
}


void CPolylineAttributeWidget::handleNoticePolylineAttribute(Polyline polyline)
{
	ui.tableWidget->blockSignals(true);

	m_polyline = polyline;
	ui.spinBox->setValue(m_polyline.numVertices);
	RefreshTable();

	ui.tableWidget->blockSignals(false);
}

