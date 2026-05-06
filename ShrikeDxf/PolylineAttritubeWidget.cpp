#include "PolylineAttritubeWidget.h"

CPolylineAttributeWidget::CPolylineAttributeWidget(QWidget* parent)
	: QWidget(parent)
{
	
}

CPolylineAttributeWidget::~CPolylineAttributeWidget()
{
}

void CPolylineAttributeWidget::RefreshTable()
{
	ui.tableWidget->setRowCount(m_polyline.vertexCount);
	ui.tableWidget->setColumnCount(2);

	QStringList header;
	header << "X" << "Y";
	ui.tableWidget->setHorizontalHeaderLabels(header);
	for (int i = 0; i < m_polyline.vertexCount; i++)
	{
		QTableWidgetItem* itemX = new QTableWidgetItem(QString::number(m_polyline.vertices[i].point.x()));
		ui.tableWidget->setItem(i, 0, itemX);
		QTableWidgetItem* itemY = new QTableWidgetItem(QString::number(m_polyline.vertices[i].point.y()));
		ui.tableWidget->setItem(i, 1, itemY);
	}

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void CPolylineAttributeWidget::handleNoticePolylineAttribute(EntityPolyline polyline)
{
	ui.tableWidget->blockSignals(true);

	m_polyline = polyline;
	ui.spinBox_Vertices->setValue(m_polyline.vertexCount);
	ui.spinBox_Linenum->setValue(m_polyline.vertexCount - 1);
	RefreshTable();

	ui.tableWidget->blockSignals(false);
}