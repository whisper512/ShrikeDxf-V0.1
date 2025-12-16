
#include "TreeView.h"
#include "ShrikeDxf.h"

#include <QMessageBox>

CTreeView::CTreeView(QWidget* pMainwnd):
	m_pMainwnd(pMainwnd),
	m_pTreeView(nullptr)
{
}

CTreeView::~CTreeView()
{
	delete m_pTreeView;
}

void CTreeView::CreateTreeView()
{
	m_pTreeView = new QTreeView(m_pMainwnd);
	
	if (m_pMainwnd)
	{
		ShrikeDxf* pWnd = dynamic_cast<ShrikeDxf*>(m_pMainwnd);
		pWnd->ui.verticalLayout_FileStructure->addWidget(m_pTreeView);
	}
}

void CTreeView::RefreshTree()
{
	//从映射类中取数据显示
}