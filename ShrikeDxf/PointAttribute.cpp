#include "PointAttribute.h"

#include <QmessageBox>

CPointAttributeWidget::CPointAttributeWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
	m_EntityPoint.point.setX(ui.doubleSpinBox_X->value());
    m_EntityPoint.point.setY(ui.doubleSpinBox_Y->value());
	emit signalPointAttributeChanged(m_EntityPoint);
}

void CPointAttributeWidget::handleNoticePointAttribute(EntityPoint point)
{
	//阻塞信号,防止触发change信号
	ui.doubleSpinBox_X->blockSignals(true);
	ui.doubleSpinBox_Y->blockSignals(true);

    m_EntityPoint = point;
	ui.doubleSpinBox_X->setValue(m_EntityPoint.point.x());
	ui.doubleSpinBox_Y->setValue(m_EntityPoint.point.y());

	ui.doubleSpinBox_X->blockSignals(false);
	ui.doubleSpinBox_Y->blockSignals(false);
}


