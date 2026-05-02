#pragma once

#include <QWidget>

#include "ui_PolylineAttributeWidget.h"
#include "DxfStruct.h"

class CPolylineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CPolylineAttributeWidget(QWidget *parent = nullptr);
	~CPolylineAttributeWidget();

private:
	Ui::CPolylineAttributeWidgetClass ui;

	EntityPolyline m_polyline;

	void RefreshTable();

	void OnTableValueChanged();

protected:


signals:
	void signalPolylineAttributeChanged(EntityPolyline polyline);


public slots:
	void handleNoticePolylineAttribute(EntityPolyline polyline);

};