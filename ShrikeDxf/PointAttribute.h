#pragma once
#include <QWidget>

#include "ui_PointAttribute.h"
#include "Primitive.h"

class CPointAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CPointAttributeWidget(QWidget *parent = nullptr);
	~CPointAttributeWidget();

private:
	Point m_point;
	Ui::CPointAttribute ui;
private:

	void OnSpinBoxChanged();


signals:
	void signalPointAttributeChanged(Point point);

public slots:
	void handleNoticePointAttribute(Point point);
};

