#pragma once
#include <QWidget>

#include "ui_PointAttribute.h"
#include "DxfStruct.h"

class CPointAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CPointAttributeWidget(QWidget *parent = nullptr);
	~CPointAttributeWidget();

private:
	EntityPoint m_EntityPoint;
	Ui::CPointAttribute ui;
private:

	void OnSpinBoxChanged();


signals:
	void signalPointAttributeChanged(EntityPoint point);

public slots:
	void handleNoticePointAttribute(EntityPoint point);
};

