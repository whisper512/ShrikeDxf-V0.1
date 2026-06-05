#include "CreateEntityWidget.h"
#include "ShrikeDxf.h"

CCreateEntityWidget::CCreateEntityWidget(QWidget* parent)
	: QWidget(parent),
	m_pMainWnd(parent),
	m_eSlectedIndex(enumMouseStateInView::enumMouseState_None)
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
    ui.toolButton_Ellipse->setIcon(QIcon(":/ShrikeDxf/res/Ellipse.png"));
	ui.toolButton_SplineFitPoint->setIcon(QIcon(":/ShrikeDxf/res/Spline.png"));
	ui.toolButton_SplineControlPoint->setIcon(QIcon(":/ShrikeDxf/res/Spline_ControlPt.png"));
    ui.toolButton_Text->setIcon(QIcon(":/ShrikeDxf/res/Text.png"));
	ui.toolButton_Mtext->setIcon(QIcon(":/ShrikeDxf/res/MText.png"));
	ui.toolButton_Rectangle->setIcon(QIcon(":/ShrikeDxf/res/Rectangle.png"));

	ui.toolButton_Mouse->setToolTip("Mouse");
    ui.toolButton_Point->setToolTip("Point");
    ui.toolButton_Line->setToolTip("Line");
    ui.toolButton_Center_Radius_Circle->setToolTip("Center Radius Circle");
    ui.toolButton_Center_Diameter_Circle->setToolTip("Diameter Circle");
    ui.toolButton_Center_Endpoint_Arc->setToolTip("Center Endpoint Arc");
    ui.toolButton_ThreePoint_Arc->setToolTip("Three Points Arc");
    ui.toolButton_Polyline->setToolTip("Polyline");
	ui.toolButton_Ellipse->setToolTip("Ellipse");
	ui.toolButton_Rectangle->setToolTip("Rectangle");
	ui.toolButton_SplineFitPoint->setToolTip("Spline Fit Point");
    ui.toolButton_SplineControlPoint->setToolTip("Spline Control Point");
	ui.toolButton_Text->setToolTip("Text");
    ui.toolButton_Mtext->setToolTip("Mtext");
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
    connect(ui.toolButton_Ellipse, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Rectangle, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_SplineFitPoint, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_SplineControlPoint, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
    connect(ui.toolButton_Text, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
	connect(ui.toolButton_Mtext, &QToolButton::clicked, this, &CCreateEntityWidget::OnToolBtnClicked);
}

void CCreateEntityWidget::RefreshBtn()
{
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

		auto setButtonSize = [CurWidth, icon](QToolButton* btn) {
			if (!btn) return;
			btn->setMinimumSize(CurWidth, CurWidth);
			btn->setMaximumSize(CurWidth, CurWidth);
			btn->setIconSize(icon);
			};

		setButtonSize(ui.toolButton_Mouse);
		setButtonSize(ui.toolButton_Point);
		setButtonSize(ui.toolButton_Line);
		setButtonSize(ui.toolButton_Center_Radius_Circle);
		setButtonSize(ui.toolButton_Center_Diameter_Circle);
		setButtonSize(ui.toolButton_Center_Endpoint_Arc);
		setButtonSize(ui.toolButton_ThreePoint_Arc);
		setButtonSize(ui.toolButton_Polyline);
		setButtonSize(ui.toolButton_Ellipse);
		setButtonSize(ui.toolButton_Rectangle);
		setButtonSize(ui.toolButton_SplineFitPoint);
		setButtonSize(ui.toolButton_SplineControlPoint);
		setButtonSize(ui.toolButton_Text);
		setButtonSize(ui.toolButton_Mtext);
	}
}

void CCreateEntityWidget::resizeEvent(QResizeEvent* event)
{
	QWidget::resizeEvent(event);
	AdjustBtn();
}


void CCreateEntityWidget::OnToolBtnClicked()
{
	QToolButton* pBtn = qobject_cast<QToolButton*>(sender());
	if(!pBtn) return;

	if (pBtn == ui.toolButton_Mouse)
	{
		m_eSlectedIndex = enumMouseStateInView::enumMouseState_None;
	}
	else if (pBtn == ui.toolButton_Point)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_Point;
	}
	else if (pBtn == ui.toolButton_Line)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_Line;
	}
	else if(pBtn == ui.toolButton_Center_Radius_Circle)
	{
		m_eSlectedIndex = enumMouseStateInView::enumMouseState_CircleCenterRadius;
	}
	else if (pBtn == ui.toolButton_Center_Diameter_Circle)
	{
		m_eSlectedIndex = enumMouseStateInView::enumMouseState_CircleDiameter;
	}
	else if (pBtn == ui.toolButton_Center_Endpoint_Arc)
	{
		m_eSlectedIndex = enumMouseStateInView::enumMouseState_ArcCenterEndpoint;
	}
	else if (pBtn == ui.toolButton_ThreePoint_Arc)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_ArcThreePoints;
	}
	else if (pBtn == ui.toolButton_Polyline)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_Polyline;
	}
	else if (pBtn == ui.toolButton_Ellipse)
	{
		m_eSlectedIndex = enumMouseStateInView::enumMouseState_EllipseCenterRadius;
	}
	else if (pBtn == ui.toolButton_Rectangle)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_Rectangle;
	}
	else if (pBtn == ui.toolButton_SplineFitPoint)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_SplineFitPoint;
	}
	else if (pBtn == ui.toolButton_SplineControlPoint)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_SplineControlPoint;
	}
	else if (pBtn == ui.toolButton_Text)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_Text;
	}
	else if (pBtn == ui.toolButton_Mtext)
	{
        m_eSlectedIndex = enumMouseStateInView::enumMouseState_MText;
	}

	emit signalMouseStatus(m_eSlectedIndex);
	RefreshBtn();
}


