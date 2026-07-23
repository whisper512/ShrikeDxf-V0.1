#include <QMessageBox>

#include "StackedWidget.h"
#include "ShrikeDxf.h"


StackedWidgetManager::StackedWidgetManager(QWidget* mainWnd) :
	m_mainWnd(mainWnd),
	m_layout(nullptr),
	m_stackedWidget(nullptr),
	m_pointAttributeWidget(nullptr),
	m_lineAttributeWidget(nullptr),
	m_circleAttributeWidget(nullptr),
	m_arcAttributeWidget(nullptr),
	m_lwPolylineAttributeWidget(nullptr),
	m_ellipseAttributeWidget(nullptr),
	m_textAttributeWidget(nullptr),
	m_mTextAttributeWidget(nullptr),
	m_polylineAttributeWidget(nullptr),
	m_splineAttributeWidget(nullptr),
    m_hatchAttributeWidget(nullptr)
{
	
}

StackedWidgetManager::~StackedWidgetManager()
{
}

void StackedWidgetManager::createStackedWidget()
{
	m_stackedWidget = new QStackedWidget(m_mainWnd);
	if (m_mainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_mainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_stackedWidget);
	}
	else
	{
		QMessageBox::information(m_stackedWidget, "TreeView Error", "MainWnd is null");
		return;
	}

	m_stackedWidget->setStyleSheet("background-color: #FFFFFF;");
	addPages();
	
	connectSignalAndSlot();
}

void StackedWidgetManager::addPages()
{

	m_pointAttributeWidget = new PointAttributeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_pointAttributeWidget);
	m_pointAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_stackedWidget->widget(0)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[0] = STR_POINT_LOWERCASE;

	m_lineAttributeWidget = new LineAttributeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_lineAttributeWidget);
	m_lineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_stackedWidget->widget(1)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[1] = STR_LINE_LOWERCASE;

    m_circleAttributeWidget = new CircleAttributeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_circleAttributeWidget);
	m_circleAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_stackedWidget->widget(2)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[2] = STR_CIRCLE_LOWERCASE;

	m_arcAttributeWidget = new ArcAttritubeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_arcAttributeWidget);
	m_arcAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_stackedWidget->widget(3)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[3] = STR_ARC_LOWERCASE;

	m_lwPolylineAttributeWidget = new LWPolylineAttributeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_lwPolylineAttributeWidget);
	m_lwPolylineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_stackedWidget->widget(4)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[4] = STR_LWPOLYLINE_LOWERCASE;

	m_ellipseAttributeWidget = new EllipseAttritubeWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_ellipseAttributeWidget);
	m_ellipseAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(5)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[5] = STR_ELLIPSE_LOWERCASE;

	m_textAttributeWidget = new CTextAttritubeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_textAttributeWidget);
    m_textAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(6)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[6] = STR_TEXT_LOWERCASE;

	m_mTextAttributeWidget = new MTextAttritubeWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_mTextAttributeWidget);
    m_mTextAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(7)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[7] = STR_MTEXT_LOWERCASE;

	m_polylineAttributeWidget = new PolylineAttributeWidget(m_stackedWidget);
	m_stackedWidget->addWidget(m_polylineAttributeWidget);
    m_polylineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(8)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[8] = STR_POLYLINE_LOWERCASE;

	m_splineAttributeWidget = new SplineAttributeWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_splineAttributeWidget);
	m_splineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(9)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[9] = STR_SPLINE_LOWERCASE;

	m_hatchAttributeWidget = new HatchAttributeWidget(m_stackedWidget);
    m_stackedWidget->addWidget(m_hatchAttributeWidget);
    m_hatchAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_stackedWidget->widget(10)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[10] = STR_HATCH_LOWERCASE;
}

void StackedWidgetManager::connectSignalAndSlot()
{
	QTimer::singleShot(0, this, [this]()
	{
			  // 点
			if (m_stackedWidget && m_pointAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalPointAttribute, m_pointAttributeWidget, &PointAttributeWidget::handleNoticePointAttribute);
				connect(m_pointAttributeWidget, &PointAttributeWidget::signalPointAttributeChanged, this, &StackedWidgetManager::signalPointChanged);
			} // 线
			if (m_stackedWidget && m_lineAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalLineAttribute, m_lineAttributeWidget, &LineAttributeWidget::handleNoticeLineAttribute);
				connect(m_lineAttributeWidget, &LineAttributeWidget::signalLineAttributeChanged, this, &StackedWidgetManager::signalLineChanged);
			} // 圆
			if (m_stackedWidget && m_circleAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalCircleAttribute, m_circleAttributeWidget, &CircleAttributeWidget::handleNoticeCircleAttribute);
				connect(m_circleAttributeWidget, &CircleAttributeWidget::signalCircleAttributeChanged, this, &StackedWidgetManager::signalCircleChanged);
			} // 圆弧
			if (m_stackedWidget && m_arcAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalArcAttribute, m_arcAttributeWidget, &ArcAttritubeWidget::handleNoticeArcAttribute);
                connect(m_arcAttributeWidget, &ArcAttritubeWidget::signalArcAttributeChanged, this, &StackedWidgetManager::signalArcChanged);
			} // 轻量多段线
			if (m_stackedWidget && m_lwPolylineAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalLWPolylineAttribute, m_lwPolylineAttributeWidget, &LWPolylineAttributeWidget::handleNoticeLWPolylineAttribute);
				connect(m_lwPolylineAttributeWidget, &LWPolylineAttributeWidget::signalLWPolylineAttributeChanged, this, &StackedWidgetManager::signalLWPolylineChanged);
			} // 椭圆
			if (m_stackedWidget && m_ellipseAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalEllipseAttribute, m_ellipseAttributeWidget, &EllipseAttritubeWidget::handleNoticeEllipseAttribute);
				connect(m_ellipseAttributeWidget, &EllipseAttritubeWidget::signalEllipseAttributeChanged, this, &StackedWidgetManager::signalEllipseChanged);
			} // 文本
			if (m_stackedWidget && m_textAttributeWidget)
			{
				connect(this , &StackedWidgetManager::signalTextAttribute, m_textAttributeWidget, &CTextAttritubeWidget::handleNoticeTextAttribute);
                connect(m_textAttributeWidget, &CTextAttritubeWidget::signalTextAttributeChanged, this, &StackedWidgetManager::signalTextChanged);
			} // 多行文本
			if (m_stackedWidget && m_mTextAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalMTextAttribute, m_mTextAttributeWidget, &MTextAttritubeWidget::handleNoticeMTextAttribute);
                connect(m_mTextAttributeWidget, &MTextAttritubeWidget::signalMTextAttributeChanged, this, &StackedWidgetManager::signalMTextChanged);
			} // 多段线
			if (m_stackedWidget && m_polylineAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalPolylineAttribute, m_polylineAttributeWidget, &PolylineAttributeWidget::handleNoticePolylineAttribute);
				connect(m_polylineAttributeWidget, &PolylineAttributeWidget::signalPolylineAttributeChanged, this, &StackedWidgetManager::signalPolylineChanged);

			} // 样条曲线
			if (m_stackedWidget && m_splineAttributeWidget)
			{
                connect(this, &StackedWidgetManager::signalSplineAttribute, m_splineAttributeWidget, &SplineAttributeWidget::handleNoticeSplineAttribute);
				connect(m_splineAttributeWidget, &SplineAttributeWidget::signalSplineAttributeChanged, this, &StackedWidgetManager::signalSplineChanged);
			} // 填充
			if (m_stackedWidget && m_hatchAttributeWidget)
			{
				connect(this, &StackedWidgetManager::signalHatchAttribute, m_hatchAttributeWidget, &HatchAttributeWidget::handleNoticeHatchAtttribute);
				connect(m_hatchAttributeWidget, &HatchAttributeWidget::signalHatchAtttributeChanged, this, &StackedWidgetManager::signalHatchChanged);
			}
			
	});
}

void StackedWidgetManager::changeWidgets()
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
	if (nIndex >= 0 && nIndex < m_stackedWidget->count())
	{
		m_stackedWidget->setCurrentIndex(nIndex);
	}
}

void StackedWidgetManager::handleRefreshStackedWidget(const SelectedEntity& SelectedEntity)
{
	m_entityType = SelectedEntity.type;

	changeWidgets();

	
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