#include "TextAttritubeWidget.h"

CTextAttritubeWidget::CTextAttritubeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // DoubleSpinBox
    connect(ui.doubleSpinBox_InsertPtX, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_InsertPtY, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_alignPtX, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_alignPtY, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Angle, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Height, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_WidthFactor, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_ObliqueAngle, &QDoubleSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);

    // SpinBox
    connect(ui.spinBox_TextGen, &QSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.spinBox_alignH, &QSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.spinBox_alignV, &QSpinBox::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);

    // LineEdit
    connect(ui.lineEdit_Text, &QLineEdit::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
    connect(ui.lineEdit_Style, &QLineEdit::editingFinished, this, &CTextAttritubeWidget::OnValueChanged);
}

CTextAttritubeWidget::~CTextAttritubeWidget()
{
}

void CTextAttritubeWidget::OnValueChanged()
{
    if (m_bUpdating) return;

    m_text.insertPoint.setX(ui.doubleSpinBox_InsertPtX->value());
    m_text.insertPoint.setY(ui.doubleSpinBox_InsertPtY->value());
    m_text.alignPoint.setX(ui.doubleSpinBox_alignPtX->value());
    m_text.alignPoint.setY(ui.doubleSpinBox_alignPtY->value());
    m_text.rotation = ui.doubleSpinBox_Angle->value();
    m_text.height = ui.doubleSpinBox_Height->value();
    m_text.widthFactor = ui.doubleSpinBox_WidthFactor->value();
    m_text.obliqueAngle = ui.doubleSpinBox_ObliqueAngle->value();
    m_text.textGen = ui.spinBox_TextGen->value();
    m_text.alignH = ui.spinBox_alignH->value();
    m_text.alignV = ui.spinBox_alignV->value();
    m_text.text = ui.lineEdit_Text->text().toStdString();
    m_text.style = ui.lineEdit_Style->text().toStdString();

    emit signalTextAttributeChanged(m_text);
}

void CTextAttritubeWidget::handleNoticeTextAttribute(EntityText text)
{
    m_bUpdating = true;

    m_text = text;
    ui.doubleSpinBox_InsertPtX->setValue(text.insertPoint.x());
    ui.doubleSpinBox_InsertPtY->setValue(text.insertPoint.y());
    ui.doubleSpinBox_alignPtX->setValue(text.alignPoint.x());
    ui.doubleSpinBox_alignPtY->setValue(text.alignPoint.y());
    ui.doubleSpinBox_Angle->setValue(text.rotation);
    ui.doubleSpinBox_Height->setValue(text.height);
    ui.doubleSpinBox_WidthFactor->setValue(text.widthFactor);
    ui.doubleSpinBox_ObliqueAngle->setValue(text.obliqueAngle);
    ui.spinBox_TextGen->setValue(text.textGen);
    ui.spinBox_alignH->setValue(text.alignH);
    ui.spinBox_alignV->setValue(text.alignV);
    ui.lineEdit_Text->setText(QString::fromStdString(text.text));
    ui.lineEdit_Style->setText(QString::fromStdString(text.style));

    m_bUpdating = false;
}
