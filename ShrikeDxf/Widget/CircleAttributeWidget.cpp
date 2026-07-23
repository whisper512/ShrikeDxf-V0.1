#include "CircleAttributeWidget.h"

CircleAttributeWidget::CircleAttributeWidget(QWidget* parent)
    : QWidget(parent), m_circle()
{
    ui.setupUi(this);

    connect(ui.doubleSpinBox_CenterX, &QDoubleSpinBox::editingFinished, this, &CircleAttributeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_CenterY, &QDoubleSpinBox::editingFinished, this, &CircleAttributeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CircleAttributeWidget::onSpinBoxChanged);
}

CircleAttributeWidget::~CircleAttributeWidget()
{
}

void CircleAttributeWidget::onSpinBoxChanged()
{
    if (m_updating) return;

    m_circle.center.setX(ui.doubleSpinBox_CenterX->value());
    m_circle.center.setY(ui.doubleSpinBox_CenterY->value());
    m_circle.radius = ui.doubleSpinBox_Radius->value();

    emit signalCircleAttributeChanged(m_circle);
}

void CircleAttributeWidget::handleNoticeCircleAttribute(EntityCircle circle)
{
    m_updating = true;

    m_circle = circle;
    ui.doubleSpinBox_CenterX->setValue(circle.center.x());
    ui.doubleSpinBox_CenterY->setValue(circle.center.y());
    ui.doubleSpinBox_Radius->setValue(circle.radius);

    m_updating = false;
}