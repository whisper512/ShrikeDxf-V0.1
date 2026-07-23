#include "LineAttribute.h"

LineAttributeWidget::LineAttributeWidget(QWidget* parent)
    : QWidget(parent), m_line()
{
    ui.setupUi(this);

    connect(ui.doubleSpinBox_StartX, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
    connect(ui.doubleSpinBox_StartY, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
    connect(ui.doubleSpinBox_EndX, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
    connect(ui.doubleSpinBox_EndY, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
    connect(ui.doubleSpinBox_Length, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
    connect(ui.doubleSpinBox_Angle, &QDoubleSpinBox::editingFinished, this, &LineAttributeWidget::onAnySpinChanged);
}

LineAttributeWidget::~LineAttributeWidget()
{
}

void LineAttributeWidget::onAnySpinChanged()
{
    if (m_updating) return;
    m_updating = true;

    double startX = ui.doubleSpinBox_StartX->value();
    double startY = ui.doubleSpinBox_StartY->value();
    double endX = ui.doubleSpinBox_EndX->value();
    double endY = ui.doubleSpinBox_EndY->value();
    double length = ui.doubleSpinBox_Length->value();
    double angle = ui.doubleSpinBox_Angle->value();

    m_line.startPoint.setX(startX);
    m_line.startPoint.setY(startY);

    QDoubleSpinBox* sender = qobject_cast<QDoubleSpinBox*>(QObject::sender());

    if (sender == ui.doubleSpinBox_Length || sender == ui.doubleSpinBox_Angle)
    {
        // 长度/角度变了
        double rad = qDegreesToRadians(angle);
        endX = startX + length * qCos(rad);
        endY = startY + length * qSin(rad);
        m_line.endPoint.setX(endX);
        m_line.endPoint.setY(endY);

        ui.doubleSpinBox_EndX->setValue(endX);
        ui.doubleSpinBox_EndY->setValue(endY);
    }
    else
    {
        // 起点/终点变了
        m_line.endPoint.setX(endX);
        m_line.endPoint.setY(endY);

        ui.doubleSpinBox_Length->setValue(m_line.length());
        ui.doubleSpinBox_Angle->setValue(m_line.angle());
    }

    m_updating = false;
    emit signalLineAttributeChanged(m_line);
}

void LineAttributeWidget::handleNoticeLineAttribute(EntityLine line)
{
    m_updating = true;     // 阻止信号递归

    m_line = line;
    ui.doubleSpinBox_StartX->setValue(line.startPoint.x());
    ui.doubleSpinBox_StartY->setValue(line.startPoint.y());
    ui.doubleSpinBox_EndX->setValue(line.endPoint.x());
    ui.doubleSpinBox_EndY->setValue(line.endPoint.y());
    ui.doubleSpinBox_Length->setValue(line.length());
    ui.doubleSpinBox_Angle->setValue(line.angle());

    m_updating = false;
}
