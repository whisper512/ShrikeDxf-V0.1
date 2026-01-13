#include "ArcAttritubeWidget.h"

CArcAttritubeWidget::CArcAttritubeWidget(QWidget* parent)
	: QWidget(parent),
	m_arc()
{
	ui.setupUi(this);

	connect(ui.doubleSpinBox_X, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Y, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Radius, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_StartAngle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_StartAngle, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_EndAngle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CArcAttritubeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_EndAngle, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
}

CArcAttritubeWidget::~CArcAttritubeWidget()
{
}

void CArcAttritubeWidget::OnSpinBoxChanged()
{
	m_arc.pointCenter.x = ui.doubleSpinBox_X->value();
    m_arc.pointCenter.y = ui.doubleSpinBox_Y->value();
	m_arc.radius = ui.doubleSpinBox_Radius->value();
	m_arc.startAngle = ui.doubleSpinBox_StartAngle->value();
	m_arc.endAngle = ui.doubleSpinBox_EndAngle->value();
	emit signalArcAttributeChanged(m_arc);
}

void CArcAttritubeWidget::handleNoticeArcAttribute(Arc arc)
{
	ui.doubleSpinBox_X->blockSignals(true);
	ui.doubleSpinBox_Y->blockSignals(true);
	ui.doubleSpinBox_Radius->blockSignals(true);
	ui.doubleSpinBox_StartAngle->blockSignals(true);
	ui.doubleSpinBox_EndAngle->blockSignals(true);

	m_arc = arc;
	ui.doubleSpinBox_X->setValue(m_arc.pointCenter.x);
	ui.doubleSpinBox_Y->setValue(m_arc.pointCenter.y);
	ui.doubleSpinBox_Radius->setValue(m_arc.radius);
	ui.doubleSpinBox_StartAngle->setValue(m_arc.startAngle);
    ui.doubleSpinBox_EndAngle->setValue(m_arc.endAngle);

	ui.doubleSpinBox_X->blockSignals(false);
	ui.doubleSpinBox_Y->blockSignals(false);
	ui.doubleSpinBox_Radius->blockSignals(false);
	ui.doubleSpinBox_StartAngle->blockSignals(false);
	ui.doubleSpinBox_EndAngle->blockSignals(false);
}