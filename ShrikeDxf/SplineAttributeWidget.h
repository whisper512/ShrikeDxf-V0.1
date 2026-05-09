#pragma once
#include <QWidget>
#include <QTimer>

#include "ui_SplineAttributeWidget.h"
#include "DxfStruct.h"

class CSplineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CSplineAttributeWidget(QWidget* parent = nullptr);
    ~CSplineAttributeWidget();

private:
    Ui::SplineAttributeWidget ui;
    EntitySpline m_pSpline;
    bool m_bUpdating = false;

    void InitSplineAttributeWidget();
    void OnValueChanged();
    void OnTableValueChanged();
    void RefreshTable();

signals:
    void signalSplineAttributeChanged(EntitySpline spline);

private slots:
    void OnTabChanged(int index);

public slots:
    void handleNoticeSplineAttribute(EntitySpline spline);
};
