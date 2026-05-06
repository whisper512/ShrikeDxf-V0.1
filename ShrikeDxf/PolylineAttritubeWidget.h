#pragma once
#include <QWidget>

#include "ui_PloylineAttributeWidget.h"
#include "DxfStruct.h"

class CPolylineAttributeWidget : public QWidget
{
public:
	CPolylineAttributeWidget(QWidget* parent = nullptr);
    ~CPolylineAttributeWidget();

private:
	Ui::PolylineAttritubeWidget ui;
	EntityPolyline m_polyline;

	void RefreshTable();

public slots:
	void handleNoticePolylineAttribute(EntityPolyline polyline);

};

