#pragma once

#include <QWidget>

#include "ui_CreateEntityWidget.h"
#include "Tools.h"

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

