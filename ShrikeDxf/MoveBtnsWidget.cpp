#include "MoveBtnsWidget.h"
#include "ShrikeDxf.h"

CMoveBtnsWidget::CMoveBtnsWidget(QWidget *parent): 
	QWidget(parent),
	m_pMainWnd(parent)
{
	ui.setupUi(this);

	connect(ui.toolButton_UP,&QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnUpClicked);
	connect(ui.toolButton_DOWN, &QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnDownClicked);
    connect(ui.toolButton_LEFT, &QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnLeftClicked);
	connect(ui.toolButton_RIGHT, &QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnRightClicked);
	connect(ui.toolButton_CW, &QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnCWClicked);
    connect(ui.toolButton_CCW, &QToolButton::clicked, this, &CMoveBtnsWidget::OnBtnCCWClicked);
}

CMoveBtnsWidget::~CMoveBtnsWidget()
{
}


void CMoveBtnsWidget::InitWidgetAndAddToLayout()
{
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(this);
	}

	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet("CMoveBtnsWidget { background-color: #d0d0d0; }"
		                "QToolButton { background-color: #e0e0e0; }");
}

void CMoveBtnsWidget::OnBtnUpClicked()
{
	emit signalOnBtnUpClicked();
}

void CMoveBtnsWidget::OnBtnDownClicked()
{
	emit signalOnBtnDownClicked();
}

void CMoveBtnsWidget::OnBtnLeftClicked()
{
	emit signalOnBtnLeftClicked();
}

void CMoveBtnsWidget::OnBtnRightClicked()
{
	emit signalOnBtnRightClicked();
}

void CMoveBtnsWidget::OnBtnCWClicked()
{
	emit signalOnBtnCWClicked();
}

void CMoveBtnsWidget::OnBtnCCWClicked()
{
    emit signalOnBtnCCWClicked();
}
