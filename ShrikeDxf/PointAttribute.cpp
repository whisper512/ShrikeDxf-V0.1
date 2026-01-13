#include "PointAttribute.h"

#include <QmessageBox>

CPointAttributeWidget::CPointAttributeWidget(QWidget *parent)
	: QWidget(parent),
	m_point(0.0,0.0,0.0)
{
	ui.setupUi(this);
	connect(ui.doubleSpinBox_X, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CPointAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &CPointAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Y, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CPointAttributeWidget::OnSpinBoxChanged);
	connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &CPointAttributeWidget::OnSpinBoxChanged);
}

CPointAttributeWidget::~CPointAttributeWidget()
{
}

void CPointAttributeWidget::OnSpinBoxChanged()
{
	m_point.x = ui.doubleSpinBox_X->value();
    m_point.y = ui.doubleSpinBox_Y->value();
	emit signalPointAttributeChanged(m_point);
}

void CPointAttributeWidget::handleNoticePointAttribute(Point point)
{
	//阻塞信号,防止触发change信号
	ui.doubleSpinBox_X->blockSignals(true);
	ui.doubleSpinBox_Y->blockSignals(true);

    m_point = point;
	ui.doubleSpinBox_X->setValue(m_point.x);
	ui.doubleSpinBox_Y->setValue(m_point.y);

	ui.doubleSpinBox_X->blockSignals(false);
	ui.doubleSpinBox_Y->blockSignals(false);
}


