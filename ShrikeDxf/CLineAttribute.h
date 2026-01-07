#pragma once

#include <QWidget>
#include "ui_CLineAttribute.h"

class CLineAttribute : public QWidget
{
	Q_OBJECT

public:
	CLineAttribute(QWidget *parent = nullptr);
	~CLineAttribute();

private:
	Ui::CLineAttributeClass ui;
};

