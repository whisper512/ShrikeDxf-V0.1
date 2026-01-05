#include <QMessageBox>

#include "StackedWidget.h"
#include "ShrikeDxf.h"
#include "Primitive.h"

CStackedWidgetManger::CStackedWidgetManger(QWidget* pMainwnd):
	m_pMainwnd(pMainwnd),
	m_pStackedWidget(nullptr),
	m_pPointAttributeClass(nullptr)
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

}

void CStackedWidgetManger::AddPages()
{
	m_pPointAttributeClass = new PointAttributeClass(m_pStackedWidget);
	m_pStackedWidget->addWidget(m_pPointAttributeClass);
	m_pPointAttributeClass->hide();
	m_mapPages[0] = STR_POINT;
}
