#include "CreateEntityWidget.h"
#include "ShrikeDxf.h"

CCreateEntityWidget::CCreateEntityWidget(QWidget *parent)
	: QWidget(parent),
	m_pMainWnd(parent)
{
	ui.setupUi(this);
}

CCreateEntityWidget::~CCreateEntityWidget()
{
}

void CCreateEntityWidget::InitWidgetAndAddToLayout()
{
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Newentity->addWidget(this);
	}

	SetIcon();
	AdjustBtn();
}

void CCreateEntityWidget::SetIcon()
{
	ui.toolButton_Mouse->setIcon(QIcon(":/ShrikeDxf/res/Mouse.png"));
	ui.toolButton_Point->setIcon(QIcon(":/ShrikeDxf/res/Point.png"));
    ui.toolButton_Line->setIcon(QIcon(":/ShrikeDxf/res/Line.png"));
    ui.toolButton_Circle->setIcon(QIcon(":/ShrikeDxf/res/Circle.png"));
    ui.toolButton_Arc->setIcon(QIcon(":/ShrikeDxf/res/Arc.png"));
    ui.toolButton_Polyline->setIcon(QIcon(":/ShrikeDxf/res/Polyline.png"));
}

void CCreateEntityWidget::AdjustBtn()
{
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		int layoutWidth = pWnd->ui.verticalLayout_Newentity->contentsRect().width();
		int CurWidth = layoutWidth;
		int iconSize = CurWidth * 0.5;
		QSize icon(iconSize, iconSize);

		QString btnStyle = "QToolButton { background-color: #E0E0E0; border: none; }"
			"QToolButton:hover { background-color: #D0D0D0; }"
			"QToolButton:pressed { background-color: #C0C0C0; }";

		ui.toolButton_Mouse->setMinimumSize(CurWidth, CurWidth);
		ui.toolButton_Mouse->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Mouse->setIconSize(icon);
        
		ui.toolButton_Mouse->setStyleSheet(btnStyle);
		ui.toolButton_Point->setMinimumSize(CurWidth, CurWidth);
		ui.toolButton_Point->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Point->setIconSize(icon);
        ui.toolButton_Point->setStyleSheet(btnStyle);

		ui.toolButton_Line->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Line->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Line->setIconSize(icon);
		ui.toolButton_Line->setStyleSheet(btnStyle);

        ui.toolButton_Circle->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Circle->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Circle->setIconSize(icon);
		ui.toolButton_Circle->setStyleSheet(btnStyle);

        ui.toolButton_Arc->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Arc->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Arc->setIconSize(icon);
		ui.toolButton_Arc->setStyleSheet(btnStyle);

        ui.toolButton_Polyline->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Polyline->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Polyline->setIconSize(icon);
		ui.toolButton_Polyline->setStyleSheet(btnStyle);
	}
}

void CCreateEntityWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	AdjustBtn();
}

