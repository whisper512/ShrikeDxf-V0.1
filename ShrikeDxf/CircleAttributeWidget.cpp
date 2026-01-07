#include "CircleAttributeWidget.h"

CCircleAttributeWidget::CCircleAttributeWidget(QWidget *parent)
	: QWidget(parent),
	m_cirlce()
{
	ui.setupUi(this);
}

CCircleAttributeWidget::~CCircleAttributeWidget()
{
}

void CCircleAttributeWidget::handleNoticeCircleAttribute(Circle circle)
{
	m_cirlce = circle;
	ui.doubleSpinBox_CenterX->setValue(m_cirlce.pointCenter.x);
	ui.doubleSpinBox_CenterY->setValue(m_cirlce.pointCenter.y);
   ui.doubleSpinBox_Radius->setValue(m_cirlce.radius);
}

