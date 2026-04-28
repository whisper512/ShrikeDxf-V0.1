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
	m_pPolylineAttributeWidget(nullptr),
	m_entityType(enumEntity_None)
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
	m_mapPages[0] = STR_POINT;

	m_pLineAttributeWidget = new CLineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pLineAttributeWidget);
	m_pLineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(1)->setContentsMargins(0, 0, 0, 0);
	m_mapPages[1] = STR_LINE;

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

	m_pPolylineAttributeWidget = new CPolylineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPolylineAttributeWidget);
	m_pPolylineAttributeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_pStackedWidget->widget(4)->setContentsMargins(0, 0, 0, 0);
    m_mapPages[4] = STR_POLYLINE_LOWERCASE;
}

void CStackedWidgetManger::ConnectSignalAndSlot()
{
	QTimer::singleShot(0, this, [this]()
	{
			if (m_pStackedWidget && m_pPointAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::NoticePointAttribute, m_pPointAttributeWidget, &CPointAttributeWidget::handleNoticePointAttribute);
			}
			if (m_pStackedWidget && m_pLineAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::NoticeLineAttribute, m_pLineAttributeWidget, &CLineAttributeWidget::handleNoticeLineAttribute);
			}
			if (m_pStackedWidget && m_pCircleAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::NoticeCircleAttribute, m_pCircleAttributeWidget, &CCircleAttributeWidget::handleNoticeCircleAttribute);
			}
			if (m_pStackedWidget && m_pArcAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::NoticeArcAttribute, m_pArcAttributeWidget, &CArcAttritubeWidget::handleNoticeArcAttribute);
			}
			if (m_pStackedWidget && m_pPolylineAttributeWidget)
			{
				connect(this, &CStackedWidgetManger::NoticePolylineAttribute, m_pPolylineAttributeWidget, &CPolylineAttributeWidget::handleNoticePolylineAttribute);
			}
	});
}

void CStackedWidgetManger::ChangeWidgets()
{
	int nIndex = -1;
	switch (m_entityType)
	{
	case enumEntity_None:
		break;
	case enumEntity_Point:
		nIndex = 0;
		break;
	case enumEntity_Line:
		nIndex = 1;
		break;
	case enumEntity_Circle:
		nIndex = 2;
		break;
	case enumEntity_Arc:
		nIndex = 3;
		break;
	case enumEntity_Polyline:
		nIndex = 4;
		break;
	case enumEntity_Text:
		break;
	default:
		break;
	}
	if (nIndex >= 0 && nIndex < m_pStackedWidget->count())
	{
		m_pStackedWidget->setCurrentIndex(nIndex);
	}
}

void CStackedWidgetManger::handleRefreshStackedWidget(variantDxfEntity dxfentity )
{
	Point point;
	Line line;
	Circle circle;
	Arc arc;
	Polyline polyline;
	

	// 使用 Lambda 捕获外部变量引用
	std::visit([&](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;

		if constexpr (std::is_same_v<T, Point>) 
		{
			point = arg;
			m_entityType = enumEntity_Point;
		}
		else if constexpr (std::is_same_v<T, Line>) 
		{
			line = arg;
			m_entityType = enumEntity_Line;
		}
		else if constexpr (std::is_same_v<T, Circle>) 
		{
			circle = arg;
			m_entityType = enumEntity_Circle;
		}
		else if constexpr (std::is_same_v<T, Arc>)
		{
			arc = arg;
			m_entityType = enumEntity_Arc;
		}
		else if constexpr (std::is_same_v<T, Polyline>)
		{
			polyline = arg;
			m_entityType = enumEntity_Polyline;
		}
		else if constexpr (std::is_same_v<T, Text>)
		{
			
		}
	}
	,dxfentity);


	
	if (m_entityType != enumEntity_None)
	{
		switch (m_entityType)
		{
		case enumEntity_Point:
			ChangeWidgets();
			emit NoticePointAttribute(point);
			break;
		case enumEntity_Line:
			ChangeWidgets();
			emit NoticeLineAttribute(line);
			break;
		case enumEntity_Circle:
			ChangeWidgets();
			emit NoticeCircleAttribute(circle);
			break;
		case enumEntity_Arc:
			ChangeWidgets();
			emit NoticeArcAttribute(arc);
			break;
		case enumEntity_Polyline:
			ChangeWidgets();
			emit NoticePolylineAttribute(polyline);
			break;
		default:
			break;
		}
	}
	
}