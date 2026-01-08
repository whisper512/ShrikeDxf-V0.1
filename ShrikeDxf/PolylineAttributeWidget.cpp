#include "PolylineAttributeWidget.h"

CPolylineAttributeWidget::CPolylineAttributeWidget(QWidget* parent)
	: QWidget(parent),
	m_polyline()
{
	ui.setupUi(this);
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

void CPolylineAttributeWidget::handleNoticePolylineAttribute(Polyline polyline)
{
	m_polyline = polyline;
	ui.spinBox->setValue(m_polyline.numVertices);
	RefreshTable();
}

