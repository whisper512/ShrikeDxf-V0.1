
#include "TreeView.h"
#include "ShrikeDxf.h"

#include <QHeaderView>
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
	m_pTreeView->header()->setVisible(false);
}

void CTreeView::RefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);
	}
}