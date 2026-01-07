#include "PointAttribute.h"

CPointAttributeWidget::CPointAttributeWidget(QWidget *parent)
	: QWidget(parent),
	m_point(0.0,0.0,0.0)
{
	ui.setupUi(this);
}

CPointAttributeWidget::~CPointAttributeWidget()
{
}

void CPointAttributeWidget::handleNoticePointAttribute(Point point)
{
    m_point = point;
	ui.doubleSpinBox_X->setValue(m_point.x);
    ui.doubleSpinBox_Y->setValue(m_point.y);
}


