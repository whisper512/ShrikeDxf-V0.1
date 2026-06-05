#include "PointAttribute.h"

CPointAttributeWidget::CPointAttributeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(ui.doubleSpinBox_X, &QDoubleSpinBox::editingFinished, this, &CPointAttributeWidget::OnSpinBoxChanged);
    connect(ui.doubleSpinBox_Y, &QDoubleSpinBox::editingFinished, this, &CPointAttributeWidget::OnSpinBoxChanged);
}

CPointAttributeWidget::~CPointAttributeWidget()
{
}

void CPointAttributeWidget::OnSpinBoxChanged()
{
    if (m_bUpdating) return;

    m_EntityPoint.point.setX(ui.doubleSpinBox_X->value());
    m_EntityPoint.point.setY(ui.doubleSpinBox_Y->value());
    emit signalPointAttributeChanged(m_EntityPoint);
}

void CPointAttributeWidget::handleNoticePointAttribute(EntityPoint point)
{
    m_bUpdating = true;

    m_EntityPoint = point;
    ui.doubleSpinBox_X->setValue(point.point.x());
    ui.doubleSpinBox_Y->setValue(point.point.y());

    m_bUpdating = false;
}
