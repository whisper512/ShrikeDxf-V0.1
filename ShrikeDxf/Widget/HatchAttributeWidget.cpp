#include "HatchAttributeWidget.h"

CHatchAttributeWidget::CHatchAttributeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    
    connect(ui.spinBox_Associative, &QSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.spinBox_Style, &QSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.spinBox_PatternType, &QSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.spinBox_DoubleFlag, &QSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.spinBox_LoopCount, &QSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Angle, &QDoubleSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_Scale, &QDoubleSpinBox::editingFinished, this, &CHatchAttributeWidget::OnValueChanged);

    connect(ui.checkBox_soildFill, &QCheckBox::clicked, this, &CHatchAttributeWidget::OnValueChanged);
}

CHatchAttributeWidget::~CHatchAttributeWidget()
{
}

void CHatchAttributeWidget::OnValueChanged()
{
    if (m_bUpdating) return;

    m_hatch.solidFill = ui.checkBox_soildFill->isChecked();
    m_hatch.associative = ui.spinBox_Associative->value();
    m_hatch.style = ui.spinBox_Style->value();
    m_hatch.patternType = ui.spinBox_PatternType->value();
    m_hatch.doubleFlag = ui.spinBox_DoubleFlag->value();
    m_hatch.loopCount = ui.spinBox_LoopCount->value();
    m_hatch.angle = ui.doubleSpinBox_Angle->value();
    m_hatch.scale = ui.doubleSpinBox_Scale->value();

    emit signalHatchAtttributeChanged(m_hatch);
}

void CHatchAttributeWidget::handleNoticeHatchAtttribute(EntityHatch hatch)
{
    m_bUpdating = true;

    m_hatch = hatch;
    ui.checkBox_soildFill->setChecked(hatch.solidFill);
    ui.spinBox_Associative->setValue(hatch.associative);
    ui.spinBox_Style->setValue(hatch.style);
    ui.spinBox_PatternType->setValue(hatch.patternType);
    ui.spinBox_DoubleFlag->setValue(hatch.doubleFlag);
    ui.spinBox_LoopCount->setValue(hatch.loopCount);
    ui.doubleSpinBox_Angle->setValue(hatch.angle);
    ui.doubleSpinBox_Scale->setValue(hatch.scale);

    m_bUpdating = false;
}
