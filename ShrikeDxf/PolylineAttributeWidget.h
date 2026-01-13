#pragma once

#include <QWidget>

#include "ui_PolylineAttributeWidget.h"
#include "Primitive.h"

class CPolylineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CPolylineAttributeWidget(QWidget *parent = nullptr);
	~CPolylineAttributeWidget();

private:
	Ui::CPolylineAttributeWidgetClass ui;

	Polyline m_polyline;

	void RefreshTable();

	void OnSpinBoxChanged();

signals:
	void signalPolylineAttributeChanged(Polyline polyline);


public slots:
	void handleNoticePolylineAttribute(Polyline polyline);

};