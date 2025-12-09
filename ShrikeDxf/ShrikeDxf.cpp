#include "ShrikeDxf.h"
#include "Menu.h"

ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	InitWindow();
	InitMenuBar();
	InitTreeView();
}

ShrikeDxf::~ShrikeDxf()
{
}

void ShrikeDxf::InitWindow()
{
	
}


void ShrikeDxf::InitMenuBar()
{
	CMenu* pMenu = new CMenu(this);

}

void ShrikeDxf::InitTreeView()
{
	//QTreeView* m_pTreeView = new QTreeView(this);
	//ui.VLayout_FileStructure->addWidget(m_pTreeView);
}



