#pragma once

#include <QWidget>
#include "ui_MoveBtnsWidget.h"

class CMoveBtnsWidget : public QWidget
{
	Q_OBJECT

public:
	CMoveBtnsWidget(QWidget *parent = nullptr);
	~CMoveBtnsWidget();

	void InitWidgetAndAddToLayout();


signals:
	void signalOnBtnUpClicked();
    void signalOnBtnDownClicked();
    void signalOnBtnLeftClicked();
    void signalOnBtnRightClicked();
	void signalOnBtnCWClicked();
	void signalOnBtnCCWClicked();

private:
	Ui::CMoveBtnsWidgetClass ui;
	QWidget* m_pMainWnd;

private:
	void OnBtnUpClicked();
    void OnBtnDownClicked();
	void OnBtnLeftClicked();
    void OnBtnRightClicked();
    void OnBtnCWClicked();
    void OnBtnCCWClicked();

};

