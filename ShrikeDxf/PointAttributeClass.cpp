#include "PointAttributeClass.h"

PointAttributeClass::PointAttributeClass(QWidget *parent)
	: QWidget(parent),
	m_point(0.0,0.0,0.0)
{
	ui.setupUi(this);
}

PointAttributeClass::~PointAttributeClass()
{
}

void PointAttributeClass::handleNoticePointAttribute(Point point)
{
    m_point = point;
	ui.doubleSpinBox_X->setValue(m_point.x);
    ui.doubleSpinBox_Y->setValue(m_point.y);
}


