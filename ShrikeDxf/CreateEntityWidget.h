#pragma once

#include <QWidget>
#include "ui_CreateEntityWidget.h"

class CCreateEntityWidget : public QWidget
{
	Q_OBJECT

public:
	CCreateEntityWidget(QWidget *parent = nullptr);
	~CCreateEntityWidget();

	void InitWidgetAndAddToLayout();
	

private:
	Ui::CCreateEntityWidgetClass ui;
	QWidget* m_pMainWnd;

	void SetIcon();
	void AdjustBtn();


protected:
	void resizeEvent(QResizeEvent *event) override;

};

