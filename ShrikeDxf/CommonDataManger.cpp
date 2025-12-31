#include "CommonDataManger.h"


CCommonDataManager::CCommonDataManager():
    m_strDxfPath(""),
    m_pMainWindow(nullptr)
{
    
}

CCommonDataManager::CCommonDataManager(QWidget* pMainWindow):
    m_strDxfPath(""),
    m_pMainWindow(pMainWindow)
{
}

CCommonDataManager::~CCommonDataManager() 
{

}


