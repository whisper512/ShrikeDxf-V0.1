#include "LWPolylineAttributeWidget.h"

LWPolylineAttributeWidget::LWPolylineAttributeWidget(QWidget* parent)
    : QWidget(parent), m_polyline()
{
    ui.setupUi(this);
    ui.spinBox_Vertices->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui.spinBox_Linenum->setButtonSymbols(QAbstractSpinBox::NoButtons);

    connect(ui.tableWidget, &QTableWidget::itemChanged,
        this, &LWPolylineAttributeWidget::onTableValueChanged);
}

LWPolylineAttributeWidget::~LWPolylineAttributeWidget()
{
}

void LWPolylineAttributeWidget::refreshTable()
{
    ui.tableWidget->setRowCount(m_polyline.numVertices());
    ui.tableWidget->setColumnCount(2);

    QStringList header;
    header << "X" << "Y";
    ui.tableWidget->setHorizontalHeaderLabels(header);

    for (int i = 0; i < m_polyline.numVertices(); i++)
    {
        const auto& pt = m_polyline.vecVertices[i].point;
        ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(pt.x())));
        ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(pt.y())));
    }

    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void LWPolylineAttributeWidget::onTableValueChanged()
{
    if (m_updating) return;

    m_polyline.vecVertices.clear();
    for (int i = 0; i < ui.tableWidget->rowCount(); i++)
    {
        PolylineVertex2D vertex;
        QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
        QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
        vertex.point.setX(itemX ? itemX->text().toDouble() : 0.0);
        vertex.point.setY(itemY ? itemY->text().toDouble() : 0.0);
        m_polyline.vecVertices.push_back(vertex);
    }
    emit signalLWPolylineAttributeChanged(m_polyline);
}


void LWPolylineAttributeWidget::handleNoticeLWPolylineAttribute(EntityLWPolyline lwpolyline)
{
    m_updating = true;

    m_polyline = lwpolyline;
    ui.spinBox_Vertices->setValue(m_polyline.numVertices());
    ui.spinBox_Linenum->setValue(m_polyline.numVertices() - 1);
    refreshTable();

    m_updating = false;
}
