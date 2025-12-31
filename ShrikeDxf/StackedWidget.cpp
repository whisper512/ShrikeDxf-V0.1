#include "StackedWidget.h"
#include "ShrikeDxf.h"

CStackedWidgetManger::CStackedWidgetManger(QWidget* pMainwnd):
	m_pMainwnd(pMainwnd),
	m_pStackedWidget(nullptr)
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
}
