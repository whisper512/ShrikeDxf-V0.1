#pragma once

#include <QWidget>
#include "ui_CreateEntityWidget.h"

class CCreateEntityWidget : public QWidget
{
	Q_OBJECT

public:
	CCreateEntityWidget(QWidget *parent = nullptr);
	~CCreateEntityWidget();

private:
	Ui::CCreateEntityWidgetClass ui;
};

