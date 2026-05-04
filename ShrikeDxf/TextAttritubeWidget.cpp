#include "TextAttritubeWidget.h"

CTextAttritubeWidget::CTextAttritubeWidget(QWidget* parent)
	: QWidget(parent)
{
    ui.setupUi(this);
}

CTextAttritubeWidget::~CTextAttritubeWidget()
{
}

void CTextAttritubeWidget::handleNoticeTextAttribute(EntityText text)
{
	m_text = text;
	ui.doubleSpinBox_InsertPtX->setValue(m_text.insertPoint.x());
    ui.doubleSpinBox_InsertPtY->setValue(m_text.insertPoint.y());
	ui.doubleSpinBox_alignPtX->setValue(m_text.alignPoint.x());
	ui.doubleSpinBox_alignPtY->setValue(m_text.alignPoint.y());
	ui.doubleSpinBox_Angle->setValue(m_text.rotation);
	ui.doubleSpinBox_Height->setValue(m_text.height);
	ui.doubleSpinBox_WidthFactor->setValue(m_text.widthFactor);
	ui.doubleSpinBox_ObliqueAngle->setValue(m_text.obliqueAngle);
	ui.spinBox_TextGen->setValue(m_text.textGen);
	ui.spinBox_alignH->setValue(m_text.alignH);
	ui.spinBox_alignV->setValue(m_text.alignV);
	ui.lineEdit_Text->setText(QString::fromStdString(m_text.text));
	ui.lineEdit_Style->setText(QString::fromStdString(m_text.style));
}