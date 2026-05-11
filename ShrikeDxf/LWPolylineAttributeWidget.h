#pragma once

#include <QWidget>

#include "ui_LWPolylineAttributeWidget.h"
#include "DxfStruct.h"

class CLWPolylineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CLWPolylineAttributeWidget(QWidget* parent = nullptr);
    ~CLWPolylineAttributeWidget();

private:
    Ui::CLWPolylineAttributeWidgetClass ui;
    EntityLWPolyline m_polyline;
    bool m_bUpdating = false;

    void RefreshTable();
    void OnTableValueChanged();

signals:
    void signalLWPolylineAttributeChanged(const EntityLWPolyline& polyline);

public slots:
    void handleNoticeLWPolylineAttribute(EntityLWPolyline lwpolyline);
};
