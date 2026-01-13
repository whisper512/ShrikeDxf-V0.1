#include "LineAttribute.h"

CLineAttributeWidget::CLineAttributeWidget(QWidget* parent)
	: QWidget(parent),
    m_line()
{
	ui.setupUi(this);


	connect(ui.doubleSpinBox_StartX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_StartX, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_StartY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_StartY, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_EndX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_EndX, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_EndY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_EndY, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxChanged);
}

CLineAttributeWidget::~CLineAttributeWidget()
{
}

void CLineAttributeWidget::OnSpinBoxChanged()
{
	m_line.pointStart.x = ui.doubleSpinBox_StartX->value();
	m_line.pointStart.y = ui.doubleSpinBox_StartY->value();
    m_line.pointEnd.x = ui.doubleSpinBox_EndX->value();
    m_line.pointEnd.y = ui.doubleSpinBox_EndY->value();
	emit signalLineAttributeChanged(m_line);
}

void CLineAttributeWidget::handleNoticeLineAttribute(Line line)
{
	ui.doubleSpinBox_StartX->blockSignals(true);
	ui.doubleSpinBox_StartY->blockSignals(true);
	ui.doubleSpinBox_EndX->blockSignals(true);
	ui.doubleSpinBox_EndY->blockSignals(true);

	m_line = line;
	ui.doubleSpinBox_StartX->setValue(m_line.pointStart.x);
    ui.doubleSpinBox_StartY->setValue(m_line.pointStart.y);
	ui.doubleSpinBox_EndX->setValue(m_line.pointEnd.x);
    ui.doubleSpinBox_EndY->setValue(m_line.pointEnd.y);

	ui.doubleSpinBox_StartX->blockSignals(false);
	ui.doubleSpinBox_StartY->blockSignals(false);
	ui.doubleSpinBox_EndX->blockSignals(false);
	ui.doubleSpinBox_EndY->blockSignals(false);
}
