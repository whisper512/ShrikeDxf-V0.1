#pragma once

#include <QWidget>

#include "ui_LWPolylineAttributeWidget.h"
#include "Manager/DxfStruct.h"

class LWPolylineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    LWPolylineAttributeWidget(QWidget* parent = nullptr);
    ~LWPolylineAttributeWidget();

private:
    Ui::CLWPolylineAttributeWidgetClass ui;
    EntityLWPolyline m_polyline;
    bool m_updating = false;

    void refreshTable();
    void onTableValueChanged();

signals:
    void signalLWPolylineAttributeChanged(const EntityLWPolyline& polyline);

public slots:
    void handleNoticeLWPolylineAttribute(EntityLWPolyline lwpolyline);
};
