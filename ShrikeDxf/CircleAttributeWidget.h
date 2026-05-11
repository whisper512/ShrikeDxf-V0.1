#pragma once

#include <QWidget>

#include "ui_CircleAttributeWidget.h"
#include "DxfStruct.h"

class CCircleAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CCircleAttributeWidget(QWidget* parent = nullptr);
    ~CCircleAttributeWidget();

private:
    Ui::CCircleAttributeWidgetClass ui;
    EntityCircle m_cirlce;
    bool m_bUpdating = false;

    void OnSpinBoxChanged();

signals:
    void signalCircleAttributeChanged(const EntityCircle& circle);

public slots:
    void handleNoticeCircleAttribute(EntityCircle circle);
};
