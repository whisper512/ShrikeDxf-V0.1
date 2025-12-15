#include "DxfManger.h"
#include "CommonDataManger.h"

CDxfManger::CDxfManger()
{
}

CDxfManger::~CDxfManger()
{
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    DL_Dxf dxf;
    
    if (!dxf.in(CCommonDataManager::QStringToStdString(strPath), &m_DxfMapping)) 
    {
        
        return false;
    }

	return true;
}
