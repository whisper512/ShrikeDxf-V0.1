#pragma once
#include <QWidget>

#include "ui_PloylineAttributeWidget.h"
#include "Manager/DxfStruct.h"

class PolylineAttributeWidget : public QWidget
{
	Q_OBJECT
public:
	PolylineAttributeWidget(QWidget* parent = nullptr);
    ~PolylineAttributeWidget();

private:
	Ui::PolylineAttritubeWidget ui;
	EntityPolyline m_polyline;

	void refreshTable();

signals:
	void signalPolylineAttributeChanged(const EntityPolyline& polyline);

public slots:
	void handleNoticePolylineAttribute(EntityPolyline polyline);

};

