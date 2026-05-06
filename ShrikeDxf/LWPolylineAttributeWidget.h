#pragma once

#include <QWidget>

#include "ui_LWPolylineAttributeWidget.h"
#include "DxfStruct.h"

class CLWPolylineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CLWPolylineAttributeWidget(QWidget *parent = nullptr);
	~CLWPolylineAttributeWidget();

private:
	Ui::CLWPolylineAttributeWidgetClass ui;

	EntityLWPolyline m_polyline;

	void RefreshTable();

	void OnTableValueChanged();

protected:


signals:
	void signalPolylineAttributeChanged(EntityLWPolyline polyline);


public slots:
	void handleNoticeLWPolylineAttribute(EntityLWPolyline polyline);

};