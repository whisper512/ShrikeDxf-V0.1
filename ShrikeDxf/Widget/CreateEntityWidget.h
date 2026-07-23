#pragma once

#include <QWidget>

#include "ui_CreateEntityWidget.h"
#include "Manager/Tools.h"

class CreateEntityWidget : public QWidget
{
	Q_OBJECT

public:
	CreateEntityWidget(QWidget *parent = nullptr);
	~CreateEntityWidget();

	void initWidgetAndAddToLayout();
	

private:
	Ui::CCreateEntityWidgetClass ui;
	QWidget* m_mainWnd;
	//当前选择的按钮
	MouseStateInView m_slectedIndex;

	void setIconAndTip();
	void adjustBtn();
	void connectBtnSignals();
	void refreshBtn();

signals:
	void signalMouseStatus(MouseStateInView state);

private slots:
	void onToolBtnClicked();

public slots:
	

protected:
	void resizeEvent(QResizeEvent *event) override;

};

