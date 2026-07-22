#pragma once

#include <QWidget>
#include "ui_MoveBtnsWidget.h"

class MoveBtnsWidget : public QWidget
{
	Q_OBJECT

public:
	MoveBtnsWidget(QWidget *parent = nullptr);
	~MoveBtnsWidget();

	void InitWidgetAndAddToLayout();
	

signals:
	void signalOnBtnUpClicked();
    void signalOnBtnDownClicked();
    void signalOnBtnLeftClicked();
    void signalOnBtnRightClicked();
	void signalOnBtnCWClicked();
	void signalOnBtnCCWClicked();
	void signalOnStepLengthOrAngleChanged(double dStepLength, double dRotationAngle);

public slots:
	void handleSetStepLengthAndAngle(const double &dStepLength,const double &dRotationAngle);
	void handleSetBtnEnabled(int nType);

private:
	Ui::CMoveBtnsWidgetClass ui;
	QWidget* m_mainWnd;

	double m_dStepLength;
    double m_dRotationAngle;

private:
	void OnBtnUpClicked();
    void OnBtnDownClicked();
	void OnBtnLeftClicked();
    void OnBtnRightClicked();
    void OnBtnCWClicked();
    void OnBtnCCWClicked();

	void OnStepLengthChanged();
	void OnRotationAngleChanged();
};

