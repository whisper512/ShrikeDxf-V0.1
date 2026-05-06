#pragma once
#include <QWidget>

#include "ui_PolylineAttributeWidget.h"
#include "DxfStruct.h"

class CPolylineAttributeWidget : public QWidget
{
public:
	CPolylineAttributeWidget(QWidget* parent = nullptr);
    ~CPolylineAttributeWidget();

private:
	Ui::CPolylineAttributeWidgetClass ui;
	EntityPolyline m_polyline;

	void RefreshTable();

public slots:
	void handleNoticePolylineAttribute(EntityPolyline polyline);

};

