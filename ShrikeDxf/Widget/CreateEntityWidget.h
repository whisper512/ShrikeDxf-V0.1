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

	void InitWidgetAndAddToLayout();
	

private:
	Ui::CCreateEntityWidgetClass ui;
	QWidget* m_pMainWnd;
	//当前选择的按钮
	enumMouseStateInView m_eSlectedIndex;

	void SetIconAndTip();
	void AdjustBtn();
	void ConnectBtnSignals();
	void RefreshBtn();

signals:
	void signalMouseStatus(enumMouseStateInView state);

private slots:
	void OnToolBtnClicked();

public slots:
	

protected:
	void resizeEvent(QResizeEvent *event) override;

};

