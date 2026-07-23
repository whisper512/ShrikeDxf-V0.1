#pragma once

#include <QWidget>
#include "ui_MoveBtnsWidget.h"

class MoveBtnsWidget : public QWidget
{
	Q_OBJECT

public:
	MoveBtnsWidget(QWidget *parent = nullptr);
	~MoveBtnsWidget();

	void initWidgetAndAddToLayout();
	

signals:
	void signalonBtnUpClicked();
    void signalonBtnDownClicked();
    void signalonBtnLeftClicked();
    void signalonBtnRightClicked();
	void signalonBtnCWClicked();
	void signalonBtnCCWClicked();
	void signalOnStepLengthOrAngleChanged(double dStepLength, double dRotationAngle);

public slots:
	void handleSetStepLengthAndAngle(const double &dStepLength,const double &dRotationAngle);
	void handleSetBtnEnabled(int nType);

private:
	Ui::CMoveBtnsWidgetClass ui;
	QWidget* m_mainWnd;

	double m_stepLength;
    double m_rotationAngle;

private:
	void onBtnUpClicked();
    void onBtnDownClicked();
	void onBtnLeftClicked();
    void onBtnRightClicked();
    void onBtnCWClicked();
    void onBtnCCWClicked();

	void onStepLengthChanged();
	void onRotationAngleChanged();
};

