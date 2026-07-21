#include "MoveBtnsWidget.h"
#include "ShrikeDxf.h"

MoveBtnsWidget::MoveBtnsWidget(QWidget *parent): 
	QWidget(parent),
	m_pMainWnd(parent),
	m_dStepLength(1.0),
    m_dRotationAngle(1.0)
{
	ui.setupUi(this);

	connect(ui.toolButton_UP,&QToolButton::clicked, this, &MoveBtnsWidget::OnBtnUpClicked);
	connect(ui.toolButton_DOWN, &QToolButton::clicked, this, &MoveBtnsWidget::OnBtnDownClicked);
    connect(ui.toolButton_LEFT, &QToolButton::clicked, this, &MoveBtnsWidget::OnBtnLeftClicked);
	connect(ui.toolButton_RIGHT, &QToolButton::clicked, this, &MoveBtnsWidget::OnBtnRightClicked);
	connect(ui.toolButton_CW, &QToolButton::clicked, this, &MoveBtnsWidget::OnBtnCWClicked);
    connect(ui.toolButton_CCW, &QToolButton::clicked, this, &MoveBtnsWidget::OnBtnCCWClicked);

	connect(ui.doubleSpinBox_stepLength, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MoveBtnsWidget::OnStepLengthChanged);
	connect(ui.doubleSpinBox_stepLength, &QDoubleSpinBox::editingFinished, this, &MoveBtnsWidget::OnStepLengthChanged);
	connect(ui.doubleSpinBox_rotateAngle, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MoveBtnsWidget::OnRotationAngleChanged);
	connect(ui.doubleSpinBox_rotateAngle, &QDoubleSpinBox::editingFinished, this, &MoveBtnsWidget::OnRotationAngleChanged);
}

MoveBtnsWidget::~MoveBtnsWidget()
{
}


void MoveBtnsWidget::InitWidgetAndAddToLayout()
{
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(this);
	}

	/*this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet("CMoveBtnsWidget { background-color: #d0d0d0; }"
		                "QToolButton { background-color: #e0e0e0; }"
		                "QDoubleSpinBox { background-color: #d0d0d0; }");*/
}

void MoveBtnsWidget::OnBtnUpClicked()
{
	emit signalOnBtnUpClicked();
}

void MoveBtnsWidget::OnBtnDownClicked()
{
	emit signalOnBtnDownClicked();
}

void MoveBtnsWidget::OnBtnLeftClicked()
{
	emit signalOnBtnLeftClicked();
}

void MoveBtnsWidget::OnBtnRightClicked()
{
	emit signalOnBtnRightClicked();
}

void MoveBtnsWidget::OnBtnCWClicked()
{
	emit signalOnBtnCWClicked();
}

void MoveBtnsWidget::OnBtnCCWClicked()
{
    emit signalOnBtnCCWClicked();
}

void MoveBtnsWidget::OnStepLengthChanged()
{
	ui.doubleSpinBox_stepLength->blockSignals(true);
    m_dStepLength = ui.doubleSpinBox_stepLength->value();
	emit  signalOnStepLengthOrAngleChanged(m_dStepLength, m_dRotationAngle);
    ui.doubleSpinBox_stepLength->blockSignals(false);
}

void MoveBtnsWidget::OnRotationAngleChanged()
{
	ui.doubleSpinBox_rotateAngle->blockSignals(true);
    m_dRotationAngle = ui.doubleSpinBox_rotateAngle->value();
	emit  signalOnStepLengthOrAngleChanged(m_dStepLength, m_dRotationAngle);
	ui.doubleSpinBox_rotateAngle->blockSignals(false);
}

void MoveBtnsWidget::handleSetStepLengthAndAngle(const double& dStepLength, const double& dRotationAngle)
{
	ui.doubleSpinBox_stepLength->blockSignals(true);
    ui.doubleSpinBox_rotateAngle->blockSignals(true);
	//弧度转角度
	m_dStepLength = dStepLength;
	m_dRotationAngle = qRadiansToDegrees(dRotationAngle);
	ui.doubleSpinBox_stepLength->setValue(m_dStepLength);
    ui.doubleSpinBox_rotateAngle->setValue(m_dRotationAngle);


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