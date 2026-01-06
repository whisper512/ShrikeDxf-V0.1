#include <QMessageBox>

#include "StackedWidget.h"
#include "ShrikeDxf.h"


CStackedWidgetManger::CStackedWidgetManger(QWidget* pMainwnd) :
	m_pMainwnd(pMainwnd),
	m_pStackedWidget(nullptr),
	m_pPointAttributeClass(nullptr),
	m_indexEntity(-1)
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
	m_pPointAttributeClass = new PointAttributeClass(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPointAttributeClass);
	m_pPointAttributeClass->hide();
	m_mapPages[0] = STR_POINT;
}

void CStackedWidgetManger::ConnectSignalAndSlot()
{
	QTimer::singleShot(0, this, [this]()
	{
			if (m_pStackedWidget && m_pPointAttributeClass)
			{
				connect(this, &CStackedWidgetManger::NoticePointAttribute, m_pPointAttributeClass, &PointAttributeClass::handleNoticePointAttribute);
			}

	});
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
			point = arg;  // 直接赋值给外部变量
			m_indexEntity = 0;
		}
		else if constexpr (std::is_same_v<T, Line>) 
		{
			line = arg;
			m_indexEntity = 1;
		}
		else if constexpr (std::is_same_v<T, Circle>) 
		{
			
			
		}
		
		}
	,dxfentity);

	switch (m_indexEntity)
	{
	case 0:
		m_pPointAttributeClass->show();
		emit NoticePointAttribute(point);
	default:
		break;
	}
	

}