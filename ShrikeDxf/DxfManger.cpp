#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
{
}

CDxfManger::~CDxfManger()
{
    delete m_pMainWnd;
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    
    return false;
}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{


    return true;
}

bool CDxfManger::NewDxfFile()
{

    return true;
}

bool CDxfManger::CloseDxfFile()
{
    ClearDxfMappingData();
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}

void CDxfManger::ClearDxfMappingData()
{
   
}