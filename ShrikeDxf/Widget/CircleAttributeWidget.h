#pragma once
#include <QWidget>

#include "ui_CircleAttributeWidget.h"
#include "Manager/DxfStruct.h"

class CircleAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CircleAttributeWidget(QWidget* parent = nullptr);
    ~CircleAttributeWidget();

private:
    Ui::CCircleAttributeWidgetClass ui;
    EntityCircle m_circle;
    bool m_updating = false;

    void onSpinBoxChanged();

signals:
    void signalCircleAttributeChanged(const EntityCircle& circle);

public slots:
    void handleNoticeCircleAttribute(EntityCircle circle);
};
