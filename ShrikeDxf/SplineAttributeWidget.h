#pragma once
#include <QWidget>

#include "ui_SplineAttributeWidget.h"
#include "DxfStruct.h"

class CSplineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CSplineAttributeWidget(QWidget *parent = nullptr);
    ~CSplineAttributeWidget();

private:
    EntitySpline m_Spline;
    Ui::SplineAttributeWidget ui;

public slots:
    void handledSplineAttritube(EntitySpline pSpline);


};