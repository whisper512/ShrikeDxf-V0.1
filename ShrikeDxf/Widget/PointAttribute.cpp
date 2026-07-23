#include "PointAttribute.h"

PointAttributeWidget::PointAttributeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &PointAttributeWidget::onSpinBoxChanged);
    connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &PointAttributeWidget::onSpinBoxChanged);
}

PointAttributeWidget::~PointAttributeWidget()
{
}

void PointAttributeWidget::onSpinBoxChanged()
{
    if (m_updating) return;

    m_EntityPoint.point.setX(ui.doubleSpinBox_X->value());
    m_EntityPoint.point.setY(ui.doubleSpinBox_Y->value());
    emit signalPointAttributeChanged(m_EntityPoint);
}

void PointAttributeWidget::handleNoticePointAttribute(EntityPoint point)
{
    m_updating = true;

    m_EntityPoint = point;
    ui.doubleSpinBox_X->setValue(point.point.x());
    ui.doubleSpinBox_Y->setValue(point.point.y());

    m_updating = false;
}
