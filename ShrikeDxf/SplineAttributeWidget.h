#pragma once
#include <QWidget>
#include <QTimer>
#include "ui_SplineAttributeWidget.h"
#include "DxfStruct.h"


class CSplineAttributeWidget : public QWidget
{
    Q_OBJECT
public:
	CSplineAttributeWidget(QWidget *parent = nullptr);
    ~CSplineAttributeWidget();

private:
    void InitSplineAttributeWidget();

    Ui::SplineAttributeWidget ui;
    EntitySpline m_pSpline;

private slots:
    void OnTabChanged(int index);

public slots:
    void handleNoticeSplineAttribute(EntitySpline spline);

};

