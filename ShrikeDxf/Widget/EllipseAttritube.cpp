#include "EllipseAttritube.h"

EllipseAttritubeWidget::EllipseAttritubeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.doubleSpinBox_x, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_y, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_majorAxisX, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_majorAxisY, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_minorAxis, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_strartAngle, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_endAngle, &QDoubleSpinBox::editingFinished, this, &EllipseAttritubeWidget::onSpinBoxChanged);
}

EllipseAttritubeWidget::~EllipseAttritubeWidget()
{
}

void EllipseAttritubeWidget::onSpinBoxChanged()
{
    if (m_updating) return;

    m_ellipse.center.setX(ui.doubleSpinBox_x->value());
    m_ellipse.center.setY(ui.doubleSpinBox_y->value());
    m_ellipse.majorAxisEndpoint.setX(ui.doubleSpinBox_majorAxisX->value());
    m_ellipse.majorAxisEndpoint.setY(ui.doubleSpinBox_majorAxisY->value());
    m_ellipse.ratio = ui.doubleSpinBox_minorAxis->value();
    m_ellipse.startParam = ui.doubleSpinBox_strartAngle->value();
    m_ellipse.endParam = ui.doubleSpinBox_endAngle->value();

    emit signalEllipseAttributeChanged(m_ellipse);
}

void EllipseAttritubeWidget::handleNoticeEllipseAttribute(EntityEllipse ellipse)
{
    m_updating = true;

    m_ellipse = ellipse;
    ui.doubleSpinBox_x->setValue(ellipse.center.x());
    ui.doubleSpinBox_y->setValue(ellipse.center.y());
    ui.doubleSpinBox_majorAxisX->setValue(ellipse.majorAxisEndpoint.x());
    ui.doubleSpinBox_majorAxisY->setValue(ellipse.majorAxisEndpoint.y());
    ui.doubleSpinBox_minorAxis->setValue(ellipse.ratio);
    ui.doubleSpinBox_strartAngle->setValue(ellipse.startParam);
    ui.doubleSpinBox_endAngle->setValue(ellipse.endParam);

    m_updating = false;
}
