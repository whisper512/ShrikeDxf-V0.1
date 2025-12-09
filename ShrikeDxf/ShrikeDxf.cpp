#include "ShrikeDxf.h"

ShrikeDxf::ShrikeDxf(QWidget *parent)
    : QMainWindow(parent),
	pMenu(nullptr)
{
    ui.setupUi(this);
	InitWindow();
	InitMenuBar();
	InitTreeView();
}

ShrikeDxf::~ShrikeDxf()
{
	delete pMenu;
}

void ShrikeDxf::InitWindow()
{
	
}


void ShrikeDxf::InitMenuBar()
{
	pMenu = new CMenu(this);
}

void ShrikeDxf::InitTreeView()
{

}



