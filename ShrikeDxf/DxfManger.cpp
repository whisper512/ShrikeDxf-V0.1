#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
    , m_DxfReader(nullptr)
    , m_DxfData(nullptr)
{
    m_DxfData = std::make_unique<CDxfData>();
    m_DxfReader = std::make_unique<CDxfReader>(m_DxfData.get());
}

CDxfManger::~CDxfManger()
{
    
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    ClearDxfMappingData();
    

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