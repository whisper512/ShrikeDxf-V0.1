#include "ArcAttritubeWidget.h"

CArcAttritubeWidget::CArcAttritubeWidget(QWidget* parent)
	: QWidget(parent),
	m_arc()
{
	ui.setupUi(this);
}

CArcAttritubeWidget::~CArcAttritubeWidget()
{
}

void CArcAttritubeWidget::handleNoticeArcAttribute(Arc arc)
{
	m_arc = arc;
	ui.doubleSpinBox_X->setValue(m_arc.pointCenter.x);
	ui.doubleSpinBox_Y->setValue(m_arc.pointCenter.y);
	ui.doubleSpinBox_Radius->setValue(m_arc.radius);
	ui.doubleSpinBox_StartAngle->setValue(m_arc.startAngle);
    ui.doubleSpinBox_EndAngle->setValue(m_arc.endAngle);
}