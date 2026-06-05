#include "CircleAttributeWidget.h"

CCircleAttributeWidget::CCircleAttributeWidget(QWidget* parent)
    : QWidget(parent), m_cirlce()
{
    ui.setupUi(this);

    connect(ui.doubleSpinBox_CenterX, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_CenterY, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
}

CCircleAttributeWidget::~CCircleAttributeWidget()
{
}

void CCircleAttributeWidget::OnSpinBoxChanged()
{
    if (m_bUpdating) return;

    m_cirlce.center.setX(ui.doubleSpinBox_CenterX->value());
    m_cirlce.center.setY(ui.doubleSpinBox_CenterY->value());
    m_cirlce.radius = ui.doubleSpinBox_Radius->value();

    emit signalCircleAttributeChanged(m_cirlce);
}

void CCircleAttributeWidget::handleNoticeCircleAttribute(EntityCircle circle)
{
    m_bUpdating = true;

    m_cirlce = circle;
    ui.doubleSpinBox_CenterX->setValue(circle.center.x());
    ui.doubleSpinBox_CenterY->setValue(circle.center.y());
    ui.doubleSpinBox_Radius->setValue(circle.radius);

    m_bUpdating = false;
}