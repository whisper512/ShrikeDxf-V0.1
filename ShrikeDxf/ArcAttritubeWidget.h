#pragma once

#include <QWidget>

#include "ui_ArcAttritubeWidget.h"
#include "DxfStruct.h"

class CArcAttritubeWidget : public QWidget
{
	Q_OBJECT

public:
	CArcAttritubeWidget(QWidget *parent = nullptr);
	~CArcAttritubeWidget();

private:
	Ui::ArcAttritubeWidgetClass ui;
	
	EntityArc m_arc;

	void OnSpinBoxXYRChanged();
	void OnSpinBoxAngleRADChanged();
    void OnSpinBoxAngleDEGChanged();
signals:
	void signalArcAttributeChanged(EntityArc arc);

public slots:
	void handleNoticeArcAttribute(EntityArc arc);
};

