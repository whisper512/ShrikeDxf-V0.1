#pragma once
#include <QWidget>

#include "ui_PointAttribute.h"
#include "Manager/DxfStruct.h"

class PointAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    PointAttributeWidget(QWidget* parent = nullptr);
    ~PointAttributeWidget();

private:
    EntityPoint m_EntityPoint;
    Ui::CPointAttribute ui;
    bool m_updating = false;

    void onSpinBoxChanged();

signals:
    void signalPointAttributeChanged(const EntityPoint& point);

public slots:
    void handleNoticePointAttribute(EntityPoint point);
};
