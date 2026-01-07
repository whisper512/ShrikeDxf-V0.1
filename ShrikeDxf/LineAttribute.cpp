#include "LineAttribute.h"

CLineAttributeWidget::CLineAttributeWidget(QWidget* parent)
	: QWidget(parent),
    m_line()
{
	ui.setupUi(this);
}

CLineAttributeWidget::~CLineAttributeWidget()
{
}

void CLineAttributeWidget::handleNoticeLineAttribute(Line line)
{
	m_line = line;
	ui.doubleSpinBox_StartX->setValue(m_line.pointStart.x);
    ui.doubleSpinBox_StartY->setValue(m_line.pointStart.y);
	ui.doubleSpinBox_EndX->setValue(m_line.pointEnd.x);
    ui.doubleSpinBox_EndY->setValue(m_line.pointEnd.y);

}
