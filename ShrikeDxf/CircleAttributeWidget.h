#pragma once

#include <QWidget>

#include "ui_CircleAttributeWidget.h"
#include "Primitive.h"

class CCircleAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CCircleAttributeWidget(QWidget *parent = nullptr);
	~CCircleAttributeWidget();

private:
	Ui::CCircleAttributeWidgetClass ui;

	Circle m_cirlce;

	void OnSpinBoxChanged();

signals:
		void signalCircleAttributeChanged(Circle circle);

public slots:
	void handleNoticeCircleAttribute(Circle circle);
};

