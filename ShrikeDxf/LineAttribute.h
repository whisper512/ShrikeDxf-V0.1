#pragma once

#include <QWidget>

#include "ui_LineAttribute.h"
#include "Primitive.h"


class CLineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CLineAttributeWidget(QWidget *parent = nullptr);
	~CLineAttributeWidget();

private:
	Ui::CLineAttribute ui;
	
	Line m_line;

public slots:
	void handleNoticeLineAttribute(Line line);

};