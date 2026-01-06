#pragma once
#include <QWidget>

#include "ui_PointAttributeClass.h"
#include "Primitive.h"

class PointAttributeClass : public QWidget
{
	Q_OBJECT

public:
	PointAttributeClass(QWidget *parent = nullptr);
	~PointAttributeClass();

private:
	Point m_point;

private:
	Ui::PointAttributeClassClass ui;

public slots:
	void handleNoticePointAttribute(Point point);
};

