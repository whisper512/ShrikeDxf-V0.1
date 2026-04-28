#include "CircleAttributeWidget.h"

CCircleAttributeWidget::CCircleAttributeWidget(QWidget *parent)
	: QWidget(parent),
	m_cirlce()
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_CenterX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CCircleAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_CenterX, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_CenterY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CCircleAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_CenterY, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Radius, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CCircleAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CCircleAttributeWidget::OnSpinBoxChanged);
}

CCircleAttributeWidget::~CCircleAttributeWidget()
{
}

void CCircleAttributeWidget::OnSpinBoxChanged()
{
	m_cirlce.pointCenter.setX(ui.doubleSpinBox_CenterX->value());
    m_cirlce.pointCenter.setY(ui.doubleSpinBox_CenterY->value());
    m_cirlce.radius = ui.doubleSpinBox_Radius->value();
	emit signalCircleAttributeChanged(m_cirlce);
}

void CCircleAttributeWidget::handleNoticeCircleAttribute(Circle circle)
{
	ui.doubleSpinBox_CenterX->blockSignals(true);
    ui.doubleSpinBox_CenterY->blockSignals(true);
    ui.doubleSpinBox_Radius->blockSignals(true);

	m_cirlce = circle;
	ui.doubleSpinBox_CenterX->setValue(m_cirlce.pointCenter.x());
	ui.doubleSpinBox_CenterY->setValue(m_cirlce.pointCenter.y());
    ui.doubleSpinBox_Radius->setValue(m_cirlce.radius);

	ui.doubleSpinBox_CenterX->blockSignals(false);
	ui.doubleSpinBox_CenterY->blockSignals(false);
	ui.doubleSpinBox_Radius->blockSignals(false);
}

