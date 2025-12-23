
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
}

void CTreeView::RefreshTree(CDxfTreeviewModel* pModel)
{
	//从映射类中取数据显示
	if (pModel)
	{
		m_pTreeView->setModel(pModel);

		QHeaderView* pHeader = m_pTreeView->header();
		int iWidth = m_pTreeView->width();
		pHeader->resizeSection(0, iWidth * 0.5);
        pHeader->resizeSection(1, iWidth * 0.5);


		m_pTreeView->expandAll();
	}
}