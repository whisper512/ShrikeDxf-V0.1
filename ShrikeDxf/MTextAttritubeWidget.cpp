#include "MTextAttritubeWidget.h"

CMTextAttritubeWidget::CMTextAttritubeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    // DoubleSpinBox
    connect(ui.doubleSpinBox_InsertPtX, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_InsertPtY, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_xAxisDirX, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_xAxisDirY, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Angle, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Height, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_WidthFactor, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_LineSpacing, &QDoubleSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);

    // SpinBox
    connect(ui.spinBox_AttachPoint, &QSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.spinBox_textDir, &QSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.spinBox_LineSpaceStyle, &QSpinBox::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);

    // LineEdit
    connect(ui.lineEdit_Text, &QLineEdit::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
    connect(ui.lineEdit_Style, &QLineEdit::editingFinished, this, &CMTextAttritubeWidget::OnValueChanged);
}

CMTextAttritubeWidget::~CMTextAttritubeWidget()
{
}

void CMTextAttritubeWidget::OnValueChanged()
{
    if (m_bUpdating) return;

    m_Mtext.insertPoint.setX(ui.doubleSpinBox_InsertPtX->value());
    m_Mtext.insertPoint.setY(ui.doubleSpinBox_InsertPtY->value());
    m_Mtext.xAxisDir.setX(ui.doubleSpinBox_xAxisDirX->value());
    m_Mtext.xAxisDir.setY(ui.doubleSpinBox_xAxisDirY->value());
    m_Mtext.rotation = ui.doubleSpinBox_Angle->value();
    m_Mtext.height = ui.doubleSpinBox_Height->value();
    m_Mtext.widthFactor = ui.doubleSpinBox_WidthFactor->value();
    m_Mtext.lineSpacing = ui.doubleSpinBox_LineSpacing->value();
    m_Mtext.attachPoint = ui.spinBox_AttachPoint->value();
    m_Mtext.textDir = ui.spinBox_textDir->value();
    m_Mtext.lineSpaceStyle = ui.spinBox_LineSpaceStyle->value();
    m_Mtext.text = ui.lineEdit_Text->text().toStdString();
    m_Mtext.style = ui.lineEdit_Style->text().toStdString();

    emit signalMTextAttributeChanged(m_Mtext);
}

void CMTextAttritubeWidget::handleNoticeMTextAttribute(EntityMText Mtext)
{
    m_bUpdating = true;

    m_Mtext = Mtext;
    ui.doubleSpinBox_InsertPtX->setValue(Mtext.insertPoint.x());
    ui.doubleSpinBox_InsertPtY->setValue(Mtext.insertPoint.y());
    ui.doubleSpinBox_xAxisDirX->setValue(Mtext.xAxisDir.x());
    ui.doubleSpinBox_xAxisDirY->setValue(Mtext.xAxisDir.y());
    ui.doubleSpinBox_Angle->setValue(Mtext.rotation);
    ui.doubleSpinBox_Height->setValue(Mtext.height);
    ui.doubleSpinBox_WidthFactor->setValue(Mtext.widthFactor);
    ui.doubleSpinBox_LineSpacing->setValue(Mtext.lineSpacing);
    ui.spinBox_AttachPoint->setValue(Mtext.attachPoint);
    ui.spinBox_textDir->setValue(Mtext.textDir);
    ui.spinBox_LineSpaceStyle->setValue(Mtext.lineSpaceStyle);
    ui.lineEdit_Text->setText(QString::fromStdString(Mtext.text));
    ui.lineEdit_Style->setText(QString::fromStdString(Mtext.style));

    m_bUpdating = false;
}
