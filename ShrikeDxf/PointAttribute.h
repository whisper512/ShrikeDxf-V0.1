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

private:
	Ui::PointAttributeClassClass ui;

public slots:
	void handleNoticePointAttribute(Point point);
};

