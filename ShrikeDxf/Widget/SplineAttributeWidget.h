#pragma once
#include <QWidget>
#include <QTimer>

#include "ui_SplineAttributeWidget.h"
#include "Manager/DxfStruct.h"

class SplineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    SplineAttributeWidget(QWidget* parent = nullptr);
    ~SplineAttributeWidget();

private:
    Ui::SplineAttributeWidget ui;
    EntitySpline m_spline;
    bool m_updating = false;

    void initSplineAttributeWidget();
    void onValueChanged();
    void onTableValueChanged();
    void refreshTable();

signals:
    void signalSplineAttributeChanged(const EntitySpline& spline);

private slots:
    void onTabChanged(int index);

public slots:
    void handleNoticeSplineAttribute(EntitySpline spline);
};
