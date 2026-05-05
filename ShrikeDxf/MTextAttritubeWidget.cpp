#include "MTextAttritubeWidget.h"

CMTextAttritubeWidget::CMTextAttritubeWidget(QWidget* parent) 
  	: QWidget(parent)
{
	ui.setupUi(this);
}


CMTextAttritubeWidget::~CMTextAttritubeWidget()
{
}

void CMTextAttritubeWidget::handleNoticeMTextAttribute(EntityMText Mtext)
{
	m_Mtext = Mtext;

	ui.doubleSpinBox_InsertPtX->blockSignals(true);
	ui.doubleSpinBox_InsertPtY->blockSignals(true);
	ui.doubleSpinBox_xAxisDirX->blockSignals(true);
	ui.doubleSpinBox_xAxisDirY->blockSignals(true);
	ui.doubleSpinBox_Angle->blockSignals(true);
	ui.doubleSpinBox_Height->blockSignals(true);
	ui.doubleSpinBox_WidthFactor->blockSignals(true);
	ui.doubleSpinBox_LineSpacing->blockSignals(true);
	ui.spinBox_AttachPoint->blockSignals(true);
	ui.spinBox_textDir->blockSignals(true);
	ui.spinBox_LineSpaceStyle->blockSignals(true);
	ui.lineEdit_Text->blockSignals(true);
	ui.lineEdit_Style->blockSignals(true);

	ui.doubleSpinBox_InsertPtX->setValue(m_Mtext.insertPoint.x());
	ui.doubleSpinBox_InsertPtY->setValue(m_Mtext.insertPoint.y());
	ui.doubleSpinBox_xAxisDirX->setValue(m_Mtext.xAxisDir.x());
	ui.doubleSpinBox_xAxisDirY->setValue(m_Mtext.xAxisDir.y());
	ui.doubleSpinBox_Angle->setValue(m_Mtext.rotation);
	ui.doubleSpinBox_Height->setValue(m_Mtext.height);
	ui.doubleSpinBox_WidthFactor->setValue(m_Mtext.widthFactor);
	ui.doubleSpinBox_LineSpacing->setValue(m_Mtext.lineSpacing);
	ui.spinBox_AttachPoint->setValue(m_Mtext.attachPoint);
	ui.spinBox_textDir->setValue(m_Mtext.textDir);
	ui.spinBox_LineSpaceStyle->setValue(m_Mtext.lineSpaceStyle);
	ui.lineEdit_Text->setText(QString::fromStdString(m_Mtext.text));
	ui.lineEdit_Style->setText(QString::fromStdString(m_Mtext.style));

	ui.doubleSpinBox_InsertPtX->blockSignals(false);
	ui.doubleSpinBox_InsertPtY->blockSignals(false);
	ui.doubleSpinBox_xAxisDirX->blockSignals(false);
	ui.doubleSpinBox_xAxisDirY->blockSignals(false);
	ui.doubleSpinBox_Angle->blockSignals(false);
	ui.doubleSpinBox_Height->blockSignals(false);
	ui.doubleSpinBox_WidthFactor->blockSignals(false);
	ui.doubleSpinBox_LineSpacing->blockSignals(false);
	ui.spinBox_AttachPoint->blockSignals(false);
	ui.spinBox_textDir->blockSignals(false);
	ui.spinBox_LineSpaceStyle->blockSignals(false);
	ui.lineEdit_Text->blockSignals(false);
	ui.lineEdit_Style->blockSignals(false);
}
