#include <QMessageBox>

#include "StackedWidget.h"
#include "ShrikeDxf.h"


CStackedWidgetManger::CStackedWidgetManger(QWidget* pMainwnd) :
	m_pMainwnd(pMainwnd),
	m_pLayout(nullptr),
	m_pStackedWidget(nullptr),
	m_pPointAttributeWidget(nullptr),
	m_pLineAttributeWidget(nullptr),
	m_pCircleAttributeWidget(nullptr),
	m_pArcAttributeWidget(nullptr),
	m_pLWPolylineAttributeWidget(nullptr),
	m_pEllipseAttributeWidget(nullptr),
	m_pTextAttributeWidget(nullptr),
	m_pMTextAttributeWidget(nullptr),
	m_pPolylineAttributeWidget(nullptr),
	m_pSplineAttributeWidget(nullptr),
    m_pHatchAttributeWidget(nullptr)
{
	
}

CStackedWidgetManger::~CStackedWidgetManger()
{
}

void CStackedWidgetManger::CreateStackedWidget()
{
	m_pStackedWidget = new QStackedWidget(m_pMainwnd);
	if (m_pMainwnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainwnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_pStackedWidget);
	}
	else
	{
		QMessageBox::information(m_pStackedWidget, "TreeView Error", "MainWnd is null");
		return;
	}

	m_pStackedWidget->setStyleSheet("background-color: #FFFFFF;");
	AddPages();
	
	ConnectSignalAndSlot();
}

void CStackedWidgetManger::AddPages()
{

	m_pPointAttributeWidget = new CPointAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPointAttributeWidget);
	m_pPointAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(0)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[0] = STR_POINT_LOWERCASE;

	m_pLineAttributeWidget = new CLineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pLineAttributeWidget);
	m_pLineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(1)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[1] = STR_LINE_LOWERCASE;

    m_pCircleAttributeWidget = new CCircleAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pCircleAttributeWidget);
	m_pCircleAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(2)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[2] = STR_CIRCLE_LOWERCASE;

	m_pArcAttributeWidget = new CArcAttritubeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pArcAttributeWidget);
	m_pArcAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(3)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[3] = STR_ARC_LOWERCASE;

	m_pLWPolylineAttributeWidget = new CLWPolylineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pLWPolylineAttributeWidget);
	m_pLWPolylineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(4)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[4] = STR_LWPOLYLINE_LOWERCASE;

	m_pEllipseAttributeWidget = new CEllipseAttritubeWidget(m_pStackedWidget);
    m_pStackedWidget->addWidget(m_pEllipseAttributeWidget);
	m_pEllipseAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(5)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[5] = STR_ELLIPSE_LOWERCASE;

	m_pTextAttributeWidget = new CTextAttritubeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pTextAttributeWidget);
    m_pTextAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(6)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[6] = STR_TEXT_LOWERCASE;

	m_pMTextAttributeWidget = new CMTextAttritubeWidget(m_pStackedWidget);
    m_pStackedWidget->addWidget(m_pMTextAttributeWidget);
    m_pMTextAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(7)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[7] = STR_MTEXT_LOWERCASE;

	m_pPolylineAttributeWidget = new CPolylineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPolylineAttributeWidget);
    m_pPolylineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(8)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[8] = STR_POLYLINE_LOWERCASE;

	m_pSplineAttributeWidget = new CSplineAttributeWidget(m_pStackedWidget);
    m_pStackedWidget->addWidget(m_pSplineAttributeWidget);
	m_pSplineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(9)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[9] = STR_SPLINE_LOWERCASE;

	m_pHatchAttributeWidget = new CHatchAttributeWidget(m_pStackedWidget);
    m_pStackedWidget->addWidget(m_pHatchAttributeWidget);
    m_pHatchAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStackedWidget->widget(10)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[10] = STR_HATCH_LOWERCASE;
}

void CStackedWidgetManger::ConnectSignalAndSlot()
{
	QTimer::singleShot(0, this, [this]()
	{
			  // 点
			if (m_pStackedWidget && m_pPointAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalPointAttribute, m_pPointAttributeWidget, &CPointAttributeWidget::handleNoticePointAttribute);
				connect(m_pPointAttributeWidget, &CPointAttributeWidget::signalPointAttributeChanged, this, &CStackedWidgetManger::signalPointChanged);
			} // 线
			if (m_pStackedWidget && m_pLineAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalLineAttribute, m_pLineAttributeWidget, &CLineAttributeWidget::handleNoticeLineAttribute);
				connect(m_pLineAttributeWidget, &CLineAttributeWidget::signalLineAttributeChanged, this, &CStackedWidgetManger::signalLineChanged);
			} // 圆
			if (m_pStackedWidget && m_pCircleAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalCircleAttribute, m_pCircleAttributeWidget, &CCircleAttributeWidget::handleNoticeCircleAttribute);
				connect(m_pCircleAttributeWidget, &CCircleAttributeWidget::signalCircleAttributeChanged, this, &CStackedWidgetManger::signalCircleChanged);
			} // 圆弧
			if (m_pStackedWidget && m_pArcAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalArcAttribute, m_pArcAttributeWidget, &CArcAttritubeWidget::handleNoticeArcAttribute);
                connect(m_pArcAttributeWidget, &CArcAttritubeWidget::signalArcAttributeChanged, this, &CStackedWidgetManger::signalArcChanged);
			} // 轻量多段线
			if (m_pStackedWidget && m_pLWPolylineAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalLWPolylineAttribute, m_pLWPolylineAttributeWidget, &CLWPolylineAttributeWidget::handleNoticeLWPolylineAttribute);
				connect(m_pLWPolylineAttributeWidget, &CLWPolylineAttributeWidget::signalLWPolylineAttributeChanged, this, &CStackedWidgetManger::signalLWPolylineChanged);
			} // 椭圆
			if (m_pStackedWidget && m_pEllipseAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalEllipseAttribute, m_pEllipseAttributeWidget, &CEllipseAttritubeWidget::handleNoticeEllipseAttribute);
				connect(m_pEllipseAttributeWidget, &CEllipseAttritubeWidget::signalEllipseAttributeChanged, this, &CStackedWidgetManger::signalEllipseChanged);
			} // 文本
			if (m_pStackedWidget && m_pTextAttributeWidget)
			{
				connect(this , &CStackedWidgetManger::signalTextAttribute, m_pTextAttributeWidget, &CTextAttritubeWidget::handleNoticeTextAttribute);
			} // 多行文本
			if (m_pStackedWidget && m_pMTextAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalMTextAttribute, m_pMTextAttributeWidget, &CMTextAttritubeWidget::handleNoticeMTextAttribute);
			} // 多段线
			if (m_pStackedWidget && m_pPolylineAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalPolylineAttribute, m_pPolylineAttributeWidget, &CPolylineAttributeWidget::handleNoticePolylineAttribute);
			} // 样条曲线
			if (m_pStackedWidget && m_pSplineAttributeWidget)
			{
                connect(this, &CStackedWidgetManger::signalSplineAttribute, m_pSplineAttributeWidget, &CSplineAttributeWidget::handleNoticeSplineAttribute);
			} // 填充
			if (m_pStackedWidget && m_pHatchAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::signalHatchAttribute, m_pHatchAttributeWidget, &CHatchAttributeWidget::handleNoticeHatchAtttribute);
			}
			
	});
}

void CStackedWidgetManger::ChangeWidgets()
{
	int nIndex = -1;
	switch (m_entityType)
	{
	case EntityType::None:
		break;
	case EntityType::Point:
		nIndex = 0;
		break;
	case EntityType::Line:
		nIndex = 1;
		break;
	case EntityType::Circle:
		nIndex = 2;
		break;
	case EntityType::Arc:
		nIndex = 3;
		break;
	case EntityType::LWPolyline:
		nIndex = 4;
		break;
	case EntityType::Ellipse:
        nIndex = 5;
		break;
	case EntityType::Text:
		nIndex = 6;
		break;
	case EntityType::MText:
        nIndex = 7;
		break;
	case EntityType::Polyline:
        nIndex = 8;
		break;
	case EntityType::Spline:
        nIndex = 9;
        break;
	case EntityType::Hatch:
        nIndex = 10;
        break;
	default:
		break;
	}
	if (nIndex >= 0 && nIndex < m_pStackedWidget->count())
	{
		m_pStackedWidget->setCurrentIndex(nIndex);
	}
}

void CStackedWidgetManger::handleRefreshStackedWidget(const stuSelectedEntity& SelectedEntity)
{
	m_entityType = SelectedEntity.type;

	ChangeWidgets();

	
	if (SelectedEntity.entityIndex < 0) return;

	switch (m_entityType)
	{
	case EntityType::Point:
	{
		const EntityPoint* pPoint = std::get_if<EntityPoint>(&SelectedEntity.entity);
		if (pPoint) emit signalPointAttribute(*pPoint);
		break;
	}
	case EntityType::Line:
	{
		const EntityLine* pLine = std::get_if<EntityLine>(&SelectedEntity.entity);
		if (pLine) emit signalLineAttribute(*pLine);
		break;
	}
	case EntityType::Circle:
	{
		const EntityCircle* pCircle = std::get_if<EntityCircle>(&SelectedEntity.entity);
		if (pCircle) emit signalCircleAttribute(*pCircle);
		break;
	}
	case EntityType::Arc:
	{
		const EntityArc* pArc = std::get_if<EntityArc>(&SelectedEntity.entity);
		if (pArc) emit signalArcAttribute(*pArc);
		break;
	}
	case EntityType::LWPolyline:
	{
		const EntityLWPolyline* pLW = std::get_if<EntityLWPolyline>(&SelectedEntity.entity);
		if (pLW) { emit signalLWPolylineAttribute(*pLW); break; }
		break;
	}
	case EntityType::Polyline:
	{
		const EntityPolyline* pPoly = std::get_if<EntityPolyline>(&SelectedEntity.entity);
		if (pPoly) { emit signalPolylineAttribute(*pPoly); break; }
		break;
	}
	case EntityType::Text:
	{
		const EntityText* pText = std::get_if<EntityText>(&SelectedEntity.entity);
		if (pText) emit signalTextAttribute(*pText);
		break;
	}
	case EntityType::MText:
	{
        const EntityMText* pMText = std::get_if<EntityMText>(&SelectedEntity.entity);
        if (pMText) emit signalMTextAttribute(*pMText);
        break;
	}
	case EntityType::Ellipse:
	{
		const EntityEllipse* pEllipse = std::get_if<EntityEllipse>(&SelectedEntity.entity);
        if(pEllipse) emit signalEllipseAttribute(*pEllipse);
        break;
	}
	case EntityType::Spline:
	{
		const EntitySpline* pSpline = std::get_if<EntitySpline>(&SelectedEntity.entity);
		if (pSpline) emit signalSplineAttribute(*pSpline);
        break;
	}
	case EntityType::Hatch:
	{
		const EntityHatch* pHatch = std::get_if<EntityHatch>(&SelectedEntity.entity);
        if (pHatch) emit signalHatchAttribute(*pHatch);
        break;
	}

	default:break;
	}
}