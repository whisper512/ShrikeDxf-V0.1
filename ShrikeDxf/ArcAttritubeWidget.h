#pragma once

#include <QWidget>

#include "ui_ArcAttritubeWidget.h"
#include "Primitive.h"

class CArcAttritubeWidget : public QWidget
{
	Q_OBJECT

public:
	CArcAttritubeWidget(QWidget *parent = nullptr);
	~CArcAttritubeWidget();

private:
	Ui::ArcAttritubeWidgetClass ui;
	
	Arc m_arc;

	void OnSpinBoxChanged();

signals:
	void signalArcAttributeChanged(Arc arc);

public slots:
	void handleNoticeArcAttribute(Arc arc);
};

