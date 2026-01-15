#include "LineAttribute.h"

CLineAttributeWidget::CLineAttributeWidget(QWidget* parent)
	: QWidget(parent),
    m_line()
{
	ui.setupUi(this);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	connect(ui.doubleSpinBox_StartX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxStartChanged);
    connect(ui.doubleSpinBox_StartX, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxStartChanged);
	connect(ui.doubleSpinBox_StartY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxStartChanged);
	connect(ui.doubleSpinBox_StartY, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxStartChanged);
	connect(ui.doubleSpinBox_EndX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxEndChanged);
	connect(ui.doubleSpinBox_EndX, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxEndChanged);
	connect(ui.doubleSpinBox_EndY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnSpinBoxEndChanged);
    connect(ui.doubleSpinBox_EndY, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnSpinBoxEndChanged);
	connect(ui.doubleSpinBox_Length, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnLengthSpinBoxChanged);
	connect(ui.doubleSpinBox_Length, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnLengthSpinBoxChanged);
	connect(ui.doubleSpinBox_Angle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CLineAttributeWidget::OnAngleSpinBoxChanged);
	connect(ui.doubleSpinBox_Angle, &QDoubleSpinBox::editingFinished, this, &CLineAttributeWidget::OnAngleSpinBoxChanged);
}

CLineAttributeWidget::~CLineAttributeWidget()
{
}



void CLineAttributeWidget::OnSpinBoxStartChanged()
{
    double startX = ui.doubleSpinBox_StartX->value();
    double startY = ui.doubleSpinBox_StartY->value();
	m_line.setStartX(startX);
	m_line.setStartY(startY);
    ui.doubleSpinBox_Length->blockSignals(true);
    ui.doubleSpinBox_Angle->blockSignals(true);
    ui.doubleSpinBox_Length->setValue(m_line.Length());
    ui.doubleSpinBox_Angle->setValue(m_line.Angle());
    ui.doubleSpinBox_Length->blockSignals(false);
    ui.doubleSpinBox_Angle->blockSignals(false);
    emit signalLineAttributeChanged(m_line);
}

void CLineAttributeWidget::OnSpinBoxEndChanged()
{
	double endX = ui.doubleSpinBox_EndX->value();
    double endY = ui.doubleSpinBox_EndY->value();
    m_line.setEndX(endX);
    m_line.setEndY(endY);
    ui.doubleSpinBox_Length->blockSignals(true);
    ui.doubleSpinBox_Angle->blockSignals(true);
    ui.doubleSpinBox_Length->setValue(m_line.Length());
    ui.doubleSpinBox_Angle->setValue(m_line.Angle());
    ui.doubleSpinBox_Length->blockSignals(false);
    ui.doubleSpinBox_Angle->blockSignals(false);
    emit signalLineAttributeChanged(m_line);
}

void CLineAttributeWidget::OnAngleSpinBoxChanged()
{
    double newAngle = ui.doubleSpinBox_Angle->value();
    double startX = m_line.StartX();
    double startY = m_line.StartY();
    double length = m_line.Length();

    double endX = startX + length * qCos(qDegreesToRadians(newAngle));
    double endY = startY + length * qSin(qDegreesToRadians(newAngle));

    m_line.setEndX(endX);
    m_line.setEndY(endY);

    
    LockOrReleaseEndSpainBoxSignal(true);
    ui.doubleSpinBox_EndX->setValue(endX);
    ui.doubleSpinBox_EndY->setValue(endY);
    LockOrReleaseEndSpainBoxSignal(false);

    emit signalLineAttributeChanged(m_line);
}

void CLineAttributeWidget::OnLengthSpinBoxChanged()
{
    double newLength = ui.doubleSpinBox_Length->value();
    double startX = m_line.StartX();
    double startY = m_line.StartY();
    double angle = m_line.Angle();
    double endX = startX + newLength * qCos(qDegreesToRadians(angle));
    double endY = startY + newLength * qSin(qDegreesToRadians(angle));

    m_line.setEndX(endX);
    m_line.setEndY(endY);

    LockOrReleaseEndSpainBoxSignal(true);
    ui.doubleSpinBox_EndX->setValue(endX);
    ui.doubleSpinBox_EndY->setValue(endY);
    LockOrReleaseEndSpainBoxSignal(false);

    emit signalLineAttributeChanged(m_line);
}

void CLineAttributeWidget::RefreshData()
{
	ui.doubleSpinBox_StartX->setValue(m_line.StartX());
	ui.doubleSpinBox_StartY->setValue(m_line.StartY());
	ui.doubleSpinBox_EndX->setValue(m_line.EndX());
	ui.doubleSpinBox_EndY->setValue(m_line.EndY());
	ui.doubleSpinBox_Length->setValue(m_line.Length());
	ui.doubleSpinBox_Angle->setValue(m_line.Angle());
}

void CLineAttributeWidget::LockOrReleaseStartSpainBoxSignal(bool lock)
{
    ui.doubleSpinBox_StartX->blockSignals(lock);
    ui.doubleSpinBox_StartY->blockSignals(lock);
}

void CLineAttributeWidget::LockOrReleaseEndSpainBoxSignal(bool lock)
{
    ui.doubleSpinBox_EndX->blockSignals(lock);
    ui.doubleSpinBox_EndY->blockSignals(lock);
}


void CLineAttributeWidget::handleNoticeLineAttribute(Line line)
{
	ui.doubleSpinBox_StartX->blockSignals(true);
	ui.doubleSpinBox_StartY->blockSignals(true);
	ui.doubleSpinBox_EndX->blockSignals(true);
	ui.doubleSpinBox_EndY->blockSignals(true);
    ui.doubleSpinBox_Length->blockSignals(true);
    ui.doubleSpinBox_Angle->blockSignals(true);

	m_line = line;
	ui.doubleSpinBox_StartX->setValue(m_line.StartX());
    ui.doubleSpinBox_StartY->setValue(m_line.StartY());
	ui.doubleSpinBox_EndX->setValue(m_line.EndX());
    ui.doubleSpinBox_EndY->setValue(m_line.EndY());
	ui.doubleSpinBox_Length->setValue(m_line.Length());
    ui.doubleSpinBox_Angle->setValue(m_line.Angle());

	ui.doubleSpinBox_StartX->blockSignals(false);
	ui.doubleSpinBox_StartY->blockSignals(false);
	ui.doubleSpinBox_EndX->blockSignals(false);
	ui.doubleSpinBox_EndY->blockSignals(false);
    ui.doubleSpinBox_Length->blockSignals(false);
    ui.doubleSpinBox_Angle->blockSignals(false);
}
