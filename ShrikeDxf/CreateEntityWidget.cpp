#include "CreateEntityWidget.h"
#include "ShrikeDxf.h"

CCreateEntityWidget::CCreateEntityWidget(QWidget* parent)
	: QWidget(parent),
	m_pMainWnd(parent),
	m_iSlectedIndex(-1)
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

	SetIconAndTip();
	ConnectBtnSignals();
	AdjustBtn();
	RefreshBtn();
}

void CCreateEntityWidget::SetIconAndTip()
{
	ui.toolButton_Mouse->setIcon(QIcon(":/ShrikeDxf/res/Mouse.png"));
	ui.toolButton_Point->setIcon(QIcon(":/ShrikeDxf/res/Point.png"));
    ui.toolButton_Line->setIcon(QIcon(":/ShrikeDxf/res/Line.png"));
	ui.toolButton_Center_Radius_Circle->setIcon(QIcon(":/ShrikeDxf/res/Center-Radius-Circle.png"));
	ui.toolButton_Center_Diameter_Circle->setIcon(QIcon(":/ShrikeDxf/res/Center-Diameter-Circle.png"));
	ui.toolButton_Center_Endpoint_Arc->setIcon(QIcon(":/ShrikeDxf/res/Center-Endpoint-Arc.png"));
	ui.toolButton_ThreePoint_Arc->setIcon(QIcon(":/ShrikeDxf/res/Three-Point-Arc.png"));
    ui.toolButton_Polyline->setIcon(QIcon(":/ShrikeDxf/res/Polyline.png"));

	ui.toolButton_Mouse->setToolTip("Mouse");
    ui.toolButton_Point->setToolTip("Point");
    ui.toolButton_Line->setToolTip("Line");
    ui.toolButton_Center_Radius_Circle->setToolTip("Center Radius Circle");
    ui.toolButton_Center_Diameter_Circle->setToolTip("Diameter Circle");
    ui.toolButton_Center_Endpoint_Arc->setToolTip("Center Endpoint Arc");
    ui.toolButton_ThreePoint_Arc->setToolTip("Three Points Arc");
    ui.toolButton_Polyline->setToolTip("Polyline");
}

void CCreateEntityWidget::ConnectBtnSignals()
{
	connect(ui.toolButton_Mouse, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Point, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Line, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Center_Radius_Circle,&QToolButton::clicked,this, &CCreateEntityWidget::OnToolBtnClicked);
	connect(ui.toolButton_Center_Diameter_Circle, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Center_Endpoint_Arc, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_ThreePoint_Arc, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Polyline, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
}

void CCreateEntityWidget::RefreshBtn()
{
	QString normalStyle = "QToolButton { background-color: #e0e0e0; border: none; }";
	QString selectedStyle = "QToolButton { background-color: #d0d0d0; border: none; }";
	ui.toolButton_Mouse->setStyleSheet(normalStyle);
	ui.toolButton_Point->setStyleSheet(normalStyle);
	ui.toolButton_Line->setStyleSheet(normalStyle);
	ui.toolButton_Center_Radius_Circle->setStyleSheet(normalStyle);
	ui.toolButton_Center_Diameter_Circle->setStyleSheet(normalStyle);
    ui.toolButton_Center_Endpoint_Arc->setStyleSheet(normalStyle);
    ui.toolButton_ThreePoint_Arc->setStyleSheet(normalStyle);
	ui.toolButton_Polyline->setStyleSheet(normalStyle);
	
	switch (m_iSlectedIndex)
	{
	case -1:
        ui.toolButton_Mouse->setStyleSheet(selectedStyle);
        break;
	case 0:
        ui.toolButton_Point->setStyleSheet(selectedStyle);
        break;
    case 1:
		ui.toolButton_Line->setStyleSheet(selectedStyle);
		break;
	case 2:
        ui.toolButton_Center_Radius_Circle->setStyleSheet(selectedStyle);
		break;
	case 3:
		ui.toolButton_Center_Diameter_Circle->setStyleSheet(selectedStyle);
		break;
	case 4:
		ui.toolButton_Center_Endpoint_Arc->setStyleSheet(selectedStyle);
		break;
	case 5:
		ui.toolButton_ThreePoint_Arc->setStyleSheet(selectedStyle);
		break;
	case 6:
        ui.toolButton_Polyline->setStyleSheet(selectedStyle);
		break;
	default:
		break;
	}
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

		ui.toolButton_Mouse->setMinimumSize(CurWidth, CurWidth);
		ui.toolButton_Mouse->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Mouse->setIconSize(icon);

		ui.toolButton_Point->setMinimumSize(CurWidth, CurWidth);
		ui.toolButton_Point->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Point->setIconSize(icon);

		ui.toolButton_Line->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Line->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Line->setIconSize(icon);

		ui.toolButton_Center_Radius_Circle->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Center_Radius_Circle->setMaximumSize(CurWidth, CurWidth);
		ui.toolButton_Center_Radius_Circle->setIconSize(icon);

        ui.toolButton_Center_Diameter_Circle->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Center_Diameter_Circle->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Center_Diameter_Circle->setIconSize(icon);

        ui.toolButton_Center_Endpoint_Arc->setMinimumSize(CurWidth, CurWidth);
		ui.toolButton_Center_Endpoint_Arc->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Center_Endpoint_Arc->setIconSize(icon);

        ui.toolButton_ThreePoint_Arc->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_ThreePoint_Arc->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_ThreePoint_Arc->setIconSize(icon);

        ui.toolButton_Polyline->setMinimumSize(CurWidth, CurWidth);
        ui.toolButton_Polyline->setMaximumSize(CurWidth, CurWidth);
        ui.toolButton_Polyline->setIconSize(icon);
	}
}

void CCreateEntityWidget::OnToolBtnClicked()
{
	QToolButton* pBtn = qobject_cast<QToolButton*>(sender());
	if(!pBtn) return;

	if (pBtn == ui.toolButton_Mouse)
	{
		m_iSlectedIndex = -1;
	}
	else if (pBtn == ui.toolButton_Point)
	{
        m_iSlectedIndex = 0;
	}
	else if (pBtn == ui.toolButton_Line)
	{
        m_iSlectedIndex = 1;
	}
	else if(pBtn == ui.toolButton_Center_Radius_Circle)
	{
		m_iSlectedIndex = 2;
	}
	else if (pBtn == ui.toolButton_Center_Diameter_Circle)
	{
        m_iSlectedIndex = 3;
	}
	else if (pBtn == ui.toolButton_Center_Endpoint_Arc)
	{
		m_iSlectedIndex = 4;
	}
	else if (pBtn == ui.toolButton_ThreePoint_Arc)
	{
        m_iSlectedIndex = 5;
	}
	else if (pBtn == ui.toolButton_Polyline)
	{
        m_iSlectedIndex = 6;
	}
	emit signalMouseStatus(m_iSlectedIndex);
	RefreshBtn();
}


void CCreateEntityWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	AdjustBtn();
}

