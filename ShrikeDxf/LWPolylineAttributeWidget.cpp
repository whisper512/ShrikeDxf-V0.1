#include "LWPolylineAttributeWidget.h"

CLWPolylineAttributeWidget::CLWPolylineAttributeWidget(QWidget* parent)
	: QWidget(parent),
	m_polyline()
{
	ui.setupUi(this);
	ui.spinBox_Vertices->setButtonSymbols(QAbstractSpinBox::NoButtons);
	ui.spinBox_Linenum->setButtonSymbols(QAbstractSpinBox::NoButtons);

	connect(ui.tableWidget, &QTableWidget::itemChanged, this, &CLWPolylineAttributeWidget::OnTableValueChanged);
}

CLWPolylineAttributeWidget::~CLWPolylineAttributeWidget()
{
}

void CLWPolylineAttributeWidget::RefreshTable()
{
	ui.tableWidget->setRowCount(m_polyline.numVertices());
	ui.tableWidget->setColumnCount(2);

	QStringList header;
	header << "X" << "Y";
	ui.tableWidget->setHorizontalHeaderLabels(header);
	for (int i = 0; i < m_polyline.numVertices(); i++)
	{
		QTableWidgetItem* itemX = new QTableWidgetItem(QString::number(m_polyline.vertices[i].point.x()));
		ui.tableWidget->setItem(i, 0, itemX);
		QTableWidgetItem* itemY = new QTableWidgetItem(QString::number(m_polyline.vertices[i].point.y()));
		ui.tableWidget->setItem(i, 1, itemY);
	}

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void CLWPolylineAttributeWidget::OnTableValueChanged()
{
	//m_polyline.vecVertices.clear();
	//for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	//{
	//	QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
	//	double x = itemX ? itemX->text().toDouble() : 0.0;

	//	QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
	//	double y = itemY ? itemY->text().toDouble() : 0.0;

	//	// 创建顶点并添加到向量中
	//	Point vertex;
	//	vertex.setX(x);
 //       vertex.setY(y);
	//	m_polyline.vecVertices.push_back(vertex);
	//}
	//emit signalPolylineAttributeChanged(m_polyline);
}


void CLWPolylineAttributeWidget::handleNoticeLWPolylineAttribute(EntityLWPolyline polyline)
{
	ui.tableWidget->blockSignals(true);

	m_polyline = polyline;
	ui.spinBox_Vertices->setValue(m_polyline.numVertices());
	ui.spinBox_Linenum->setValue(m_polyline.numVertices() - 1);
	RefreshTable();

	ui.tableWidget->blockSignals(false);
}

