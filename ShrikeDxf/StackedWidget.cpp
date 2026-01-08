#include <QMessageBox>

#include "StackedWidget.h"
#include "ShrikeDxf.h"


CStackedWidgetManger::CStackedWidgetManger(QWidget* pMainwnd) :
	m_pMainwnd(pMainwnd),
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

	m_pStackedWidget->setStyleSheet("background-color: #d0d0d0;");


	AddPages();

	ConnectSignalAndSlot();
}

void CStackedWidgetManger::AddPages()
{
	m_pPointAttributeWidget = new CPointAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPointAttributeWidget);
	m_pPointAttributeWidget->hide();
	m_mapPages[0] = STR_POINT;

	m_pLineAttributeWidget = new CLineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pLineAttributeWidget);
	m_pLineAttributeWidget->hide();
	m_mapPages[1] = STR_LINE;

    m_pCircleAttributeWidget = new CCircleAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pCircleAttributeWidget);
    m_pCircleAttributeWidget->hide();
	m_mapPages[2] = STR_CIRCLE_LOWERCASE;

	m_pArcAttributeWidget = new CArcAttritubeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pArcAttributeWidget);
	m_pArcAttributeWidget->hide();
	m_mapPages[3] = STR_ARC_LOWERCASE;

	m_pPolylineAttributeWidget = new CPolylineAttributeWidget(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPolylineAttributeWidget);
	m_pPolylineAttributeWidget->hide();
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

void CStackedWidgetManger::AdjustWidget()
{
	if (m_pStackedWidget)
	{
		QSize size = m_pStackedWidget->size();
		if (m_pPointAttributeWidget)
		{
			m_pPointAttributeWidget->resize(size);
		}
		if (m_pLineAttributeWidget)
		{
			m_pLineAttributeWidget->resize(size);
		}
		if (m_pCircleAttributeWidget)
		{
			m_pCircleAttributeWidget->resize(size);
		}
		if (m_pArcAttributeWidget)
		{
			m_pArcAttributeWidget->resize(size);
		}
		if (m_pPolylineAttributeWidget)
		{
			m_pPolylineAttributeWidget->resize(size);
		}
	}
}

void CStackedWidgetManger::ChangeWidgets()
{
	m_pPointAttributeWidget->hide();
	m_pLineAttributeWidget->hide();
	m_pCircleAttributeWidget->hide();
	m_pArcAttributeWidget->hide();
	m_pPolylineAttributeWidget->hide();

	switch (m_entityType)
	{
	case enumEntity_None:
		break;
	case enumEntity_Point:
		m_pPointAttributeWidget->show();
		break;
	case enumEntity_Line:
		m_pLineAttributeWidget->show();
		break;
	case enumEntity_Circle:
        m_pCircleAttributeWidget->show();
		break;
	case enumEntity_Arc:
		m_pArcAttributeWidget->show();
		break;
	case enumEntity_Polyline:
		m_pPolylineAttributeWidget->show();
		break;
	case enumEntity_Text:
		break;
	default:
		break;
	}
}

void CStackedWidgetManger::handleRefreshStackedWidget(DxfEntity dxfentity )
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


	AdjustWidget();

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