#include "ArcAttritubeWidget.h"

CArcAttritubeWidget::CArcAttritubeWidget(QWidget* parent)
	: QWidget(parent),
	m_arc()
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_X, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_Y, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_Radius, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxXYRChanged);
	connect(ui.doubleSpinBox_StartAngle_RAD, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxAngleRADChanged);
	connect(ui.doubleSpinBox_StartAngle_RAD, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxAngleRADChanged);
	connect(ui.doubleSpinBox_EndAngle_RAD, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxAngleRADChanged);
	connect(ui.doubleSpinBox_EndAngle_RAD, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxAngleRADChanged);
	connect(ui.doubleSpinBox_StartAngle_DEG, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxAngleDEGChanged);
	connect(ui.doubleSpinBox_StartAngle_DEG, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxAngleDEGChanged);
    connect(ui.doubleSpinBox_EndAngle_DEG, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxAngleDEGChanged);
	connect(ui.doubleSpinBox_EndAngle_DEG, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxAngleDEGChanged);
}

CArcAttritubeWidget::~CArcAttritubeWidget()
{
}

void CArcAttritubeWidget::OnSpinBoxXYRChanged()
{
	m_arc.pointCenter.setX(ui.doubleSpinBox_X->value());
	m_arc.pointCenter.setY(ui.doubleSpinBox_Y->value());
	m_arc.radius = ui.doubleSpinBox_Radius->value();
	emit signalArcAttributeChanged(m_arc);
}

void CArcAttritubeWidget::OnSpinBoxAngleRADChanged()
{
	m_arc.startAngle = ui.doubleSpinBox_StartAngle_RAD->value() * 180.0 /M_PI;
	m_arc.endAngle = ui.doubleSpinBox_EndAngle_RAD->value() * 180.0 /M_PI;
	emit signalArcAttributeChanged(m_arc);
	ui.doubleSpinBox_StartAngle_DEG->setValue(m_arc.startAngle);
	ui.doubleSpinBox_EndAngle_DEG->setValue(m_arc.endAngle);
}

void CArcAttritubeWidget::OnSpinBoxAngleDEGChanged()
{
	m_arc.startAngle = ui.doubleSpinBox_StartAngle_DEG->value();
	m_arc.endAngle = ui.doubleSpinBox_EndAngle_DEG->value();
	emit signalArcAttributeChanged(m_arc);
	ui.doubleSpinBox_StartAngle_RAD->setValue(m_arc.startAngle * M_PI / 180.0);
	ui.doubleSpinBox_EndAngle_RAD->setValue(m_arc.endAngle * M_PI / 180.0);
}

void CArcAttritubeWidget::handleNoticeArcAttribute(Arc arc)
{
	ui.doubleSpinBox_X->blockSignals(true);
	ui.doubleSpinBox_Y->blockSignals(true);
	ui.doubleSpinBox_Radius->blockSignals(true);
	ui.doubleSpinBox_StartAngle_RAD->blockSignals(true);
	ui.doubleSpinBox_EndAngle_RAD->blockSignals(true);
    ui.doubleSpinBox_StartAngle_DEG->blockSignals(true);
    ui.doubleSpinBox_EndAngle_DEG->blockSignals(true);

	m_arc = arc;
	ui.doubleSpinBox_X->setValue(m_arc.pointCenter.x());
	ui.doubleSpinBox_Y->setValue(m_arc.pointCenter.y());
	ui.doubleSpinBox_Radius->setValue(m_arc.radius);
	ui.doubleSpinBox_StartAngle_DEG->setValue(m_arc.startAngle);
    ui.doubleSpinBox_EndAngle_DEG->setValue(m_arc.endAngle);
	ui.doubleSpinBox_EndAngle_RAD->setValue(m_arc.endAngle * M_PI / 180.0);
    ui.doubleSpinBox_StartAngle_RAD->setValue(m_arc.startAngle * M_PI / 180.0);

	ui.doubleSpinBox_X->blockSignals(false);
	ui.doubleSpinBox_Y->blockSignals(false);
	ui.doubleSpinBox_Radius->blockSignals(false);
	ui.doubleSpinBox_StartAngle_RAD->blockSignals(false);
	ui.doubleSpinBox_EndAngle_RAD->blockSignals(false);
    ui.doubleSpinBox_StartAngle_DEG->blockSignals(false);
    ui.doubleSpinBox_EndAngle_DEG->blockSignals(false);
}