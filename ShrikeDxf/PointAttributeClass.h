#pragma once

#include <QWidget>
#include "ui_PointAttributeClass.h"

class PointAttributeClass : public QWidget
{
	Q_OBJECT

public:
	PointAttributeClass(QWidget *parent = nullptr);
	~PointAttributeClass();

private:
	Ui::PointAttributeClassClass ui;
};

