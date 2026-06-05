#include "ArcAttritubeWidget.h"

CArcAttritubeWidget::CArcAttritubeWidget(QWidget* parent)
    : QWidget(parent), m_arc()
{
    ui.setupUi(this);

    
    connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_Radius, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_StartAngle_RAD, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_EndAngle_RAD, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_StartAngle_DEG, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_EndAngle_DEG, &QDoubleSpinBox::editingFinished, this, &CArcAttritubeWidget::OnSpinBoxChanged);
}

CArcAttritubeWidget::~CArcAttritubeWidget()
{
}

void CArcAttritubeWidget::OnSpinBoxChanged()
{
    if (m_bUpdating) return;
    m_bUpdating = true;

    QDoubleSpinBox* sender = qobject_cast<QDoubleSpinBox*>(QObject::sender());

    
    m_arc.center.setX(ui.doubleSpinBox_X->value());
    m_arc.center.setY(ui.doubleSpinBox_Y->value());
    m_arc.radius = ui.doubleSpinBox_Radius->value();

    if (sender == ui.doubleSpinBox_StartAngle_DEG || sender == ui.doubleSpinBox_EndAngle_DEG)
    {
        
        double startDeg = ui.doubleSpinBox_StartAngle_DEG->value();
        double endDeg = ui.doubleSpinBox_EndAngle_DEG->value();
        m_arc.startAngle = startDeg * M_PI / 180.0;
        m_arc.endAngle = endDeg * M_PI / 180.0;

        
        ui.doubleSpinBox_StartAngle_RAD->setValue(m_arc.startAngle);
        ui.doubleSpinBox_EndAngle_RAD->setValue(m_arc.endAngle);
    }
    else if (sender == ui.doubleSpinBox_StartAngle_RAD || sender == ui.doubleSpinBox_EndAngle_RAD)
    {
        m_arc.startAngle = ui.doubleSpinBox_StartAngle_RAD->value();
        m_arc.endAngle = ui.doubleSpinBox_EndAngle_RAD->value();

        ui.doubleSpinBox_StartAngle_DEG->setValue(m_arc.startAngle * 180.0 / M_PI);
        ui.doubleSpinBox_EndAngle_DEG->setValue(m_arc.endAngle * 180.0 / M_PI);
    }

    m_bUpdating = false;
    emit signalArcAttributeChanged(m_arc);
}

void CArcAttritubeWidget::handleNoticeArcAttribute(EntityArc arc)
{
    m_bUpdating = true;

    m_arc = arc;
    ui.doubleSpinBox_X->setValue(arc.center.x());
    ui.doubleSpinBox_Y->setValue(arc.center.y());
    ui.doubleSpinBox_Radius->setValue(arc.radius);
    ui.doubleSpinBox_StartAngle_RAD->setValue(arc.startAngle);
    ui.doubleSpinBox_EndAngle_RAD->setValue(arc.endAngle);
    ui.doubleSpinBox_StartAngle_DEG->setValue(arc.startAngle * 180.0 / M_PI);
    ui.doubleSpinBox_EndAngle_DEG->setValue(arc.endAngle * 180.0 / M_PI);

    m_bUpdating = false;
}
