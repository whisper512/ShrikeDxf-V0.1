#include "TableView.h"
#include "ShrikeDxf.h"

CTableViewManger::CTableViewManger(QWidget* pMainwnd):
    m_pMainWnd(pMainwnd),
    m_pTableView(nullptr)
{
}

CTableViewManger::~CTableViewManger()
{
}

void CTableViewManger::CreateTableView()
{
    m_pTableView = new QTableView(m_pMainWnd);
	if (m_pMainWnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainWnd);
		pWnd->ui.verticalLayout_Layer->addWidget(m_pTableView);
	}


}
