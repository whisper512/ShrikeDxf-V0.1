#include "HatchAttributeWidget.h"

CHatchAttributeWidget::CHatchAttributeWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

CHatchAttributeWidget::~CHatchAttributeWidget()
{
}

void CHatchAttributeWidget::handleNoticeHatchAtttribute(EntityHatch hatch)
{
	ui.checkBox_soildFill->blockSignals(true);
	ui.spinBox_Associative->blockSignals(true);
	ui.spinBox_Style->blockSignals(true);
	ui.spinBox_PatternType->blockSignals(true);
	ui.spinBox_DoubleFlag->blockSignals(true);
	ui.spinBox_LoopCount->blockSignals(true);
	ui.doubleSpinBox_Angle->blockSignals(true);
	ui.doubleSpinBox_Scale->blockSignals(true);

	m_hatch = hatch;
	ui.checkBox_soildFill->setChecked(hatch.solidFill);
	ui.spinBox_Associative->setValue(hatch.associative);
	ui.spinBox_Style->setValue(hatch.style);
	ui.spinBox_PatternType->setValue(hatch.patternType);
	ui.spinBox_DoubleFlag->setValue(hatch.doubleFlag);
	ui.spinBox_LoopCount->setValue(hatch.loopCount);
	ui.doubleSpinBox_Angle->setValue(hatch.angle);
    ui.doubleSpinBox_Scale->setValue(hatch.scale);

	ui.checkBox_soildFill->blockSignals(false);
	ui.spinBox_Associative->blockSignals(false);
	ui.spinBox_Style->blockSignals(false);
	ui.spinBox_PatternType->blockSignals(false);
	ui.spinBox_DoubleFlag->blockSignals(false);
	ui.spinBox_LoopCount->blockSignals(false);
	ui.doubleSpinBox_Angle->blockSignals(false);
	ui.doubleSpinBox_Scale->blockSignals(false);
}