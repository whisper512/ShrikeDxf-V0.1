#pragma once
#include <QWidget>

#include "ui_PloylineAttributeWidget.h"
#include "DxfStruct.h"

class CPolylineAttributeWidget : public QWidget
{
	Q_OBJECT
public:
	CPolylineAttributeWidget(QWidget* parent = nullptr);
    ~CPolylineAttributeWidget();

private:
	Ui::PolylineAttritubeWidget ui;
	EntityPolyline m_polyline;

	void RefreshTable();

signals:
	void signalPolylineAttributeChanged(EntityPolyline polyline);

public slots:
	void handleNoticePolylineAttribute(EntityPolyline polyline);

};

