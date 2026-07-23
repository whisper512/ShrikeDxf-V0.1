#include "MoveBtnsWidget.h"
#include "ShrikeDxf.h"

MoveBtnsWidget::MoveBtnsWidget(QWidget *parent): 
	QWidget(parent),
	m_mainWnd(parent),
	m_stepLength(1.0),
    m_rotationAngle(1.0)
{
	ui.setupUi(this);

	connect(ui.toolButton_UP,&QToolButton::clicked, this, &MoveBtnsWidget::onBtnUpClicked);
	connect(ui.toolButton_DOWN, &QToolButton::clicked, this, &MoveBtnsWidget::onBtnDownClicked);
    connect(ui.toolButton_LEFT, &QToolButton::clicked, this, &MoveBtnsWidget::onBtnLeftClicked);
	connect(ui.toolButton_RIGHT, &QToolButton::clicked, this, &MoveBtnsWidget::onBtnRightClicked);
	connect(ui.toolButton_CW, &QToolButton::clicked, this, &MoveBtnsWidget::onBtnCWClicked);
    connect(ui.toolButton_CCW, &QToolButton::clicked, this, &MoveBtnsWidget::onBtnCCWClicked);

	connect(ui.doubleSpinBox_stepLength, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MoveBtnsWidget::onStepLengthChanged);
	connect(ui.doubleSpinBox_stepLength, &QDoubleSpinBox::editingFinished, this, &MoveBtnsWidget::onStepLengthChanged);
	connect(ui.doubleSpinBox_rotateAngle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MoveBtnsWidget::onRotationAngleChanged);
	connect(ui.doubleSpinBox_rotateAngle, &QDoubleSpinBox::editingFinished, this, &MoveBtnsWidget::onRotationAngleChanged);
}

MoveBtnsWidget::~MoveBtnsWidget()
{
}


void MoveBtnsWidget::initWidgetAndAddToLayout()
{
	if (m_mainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_mainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(this);
	}

	/*this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet("CMoveBtnsWidget { background-color: #d0d0d0; }"
		                "QToolButton { background-color: #e0e0e0; }"
		                "QDoubleSpinBox { background-color: #d0d0d0; }");*/
}

void MoveBtnsWidget::onBtnUpClicked()
{
	emit signalonBtnUpClicked();
}

void MoveBtnsWidget::onBtnDownClicked()
{
	emit signalonBtnDownClicked();
}

void MoveBtnsWidget::onBtnLeftClicked()
{
	emit signalonBtnLeftClicked();
}

void MoveBtnsWidget::onBtnRightClicked()
{
	emit signalonBtnRightClicked();
}

void MoveBtnsWidget::onBtnCWClicked()
{
	emit signalonBtnCWClicked();
}

void MoveBtnsWidget::onBtnCCWClicked()
{
    emit signalonBtnCCWClicked();
}

void MoveBtnsWidget::onStepLengthChanged()
{
	ui.doubleSpinBox_stepLength->blockSignals(true);
    m_stepLength = ui.doubleSpinBox_stepLength->value();
	emit  signalOnStepLengthOrAngleChanged(m_stepLength, m_rotationAngle);
    ui.doubleSpinBox_stepLength->blockSignals(false);
}

void MoveBtnsWidget::onRotationAngleChanged()
{
	ui.doubleSpinBox_rotateAngle->blockSignals(true);
    m_rotationAngle = ui.doubleSpinBox_rotateAngle->value();
	emit  signalOnStepLengthOrAngleChanged(m_stepLength, m_rotationAngle);
	ui.doubleSpinBox_rotateAngle->blockSignals(false);
}

void MoveBtnsWidget::handleSetStepLengthAndAngle(const double& dStepLength, const double& dRotationAngle)
{
	ui.doubleSpinBox_stepLength->blockSignals(true);
    ui.doubleSpinBox_rotateAngle->blockSignals(true);
	//弧度转角度
	m_stepLength = dStepLength;
	m_rotationAngle = qRadiansToDegrees(dRotationAngle);
	ui.doubleSpinBox_stepLength->setValue(m_stepLength);
    ui.doubleSpinBox_rotateAngle->setValue(m_rotationAngle);


	ui.doubleSpinBox_stepLength->blockSignals(false);
	ui.doubleSpinBox_rotateAngle->blockSignals(false);
}

void MoveBtnsWidget::handleSetBtnEnabled(int nType)
{
	switch (nType)
	{
		//point
	case 0:
	{
		ui.doubleSpinBox_rotateAngle->setEnabled(false);
		ui.toolButton_CW->setEnabled(false);
		ui.toolButton_CCW->setEnabled(false);
		break;
	}
		//line
	case 1:
	{
		ui.doubleSpinBox_rotateAngle->setEnabled(true);
		ui.toolButton_CW->setEnabled(true);
		ui.toolButton_CCW->setEnabled(true);
		break;
	}
		//circle
	case 2:
	{
		ui.doubleSpinBox_rotateAngle->setEnabled(false);
		ui.toolButton_CW->setEnabled(false);
		ui.toolButton_CCW->setEnabled(false);
		break;
	}
		//arc
	case 3:
	{
		ui.doubleSpinBox_rotateAngle->setEnabled(true);
		ui.toolButton_CW->setEnabled(true);
		ui.toolButton_CCW->setEnabled(true);
		break;
	}
		//polyline
	case 4:
	{
		ui.doubleSpinBox_rotateAngle->setEnabled(true);
		ui.toolButton_CW->setEnabled(true);
		ui.toolButton_CCW->setEnabled(true);
		break;
	}
	case 5:
		break;
	default:
		break;
	}
}