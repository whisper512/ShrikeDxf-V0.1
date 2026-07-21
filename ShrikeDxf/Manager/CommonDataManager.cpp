#include "CommonDataManager.h"


CommonDataManager::CommonDataManager():
    m_dxfPath(""),
    m_mainWindow(nullptr)
{
    
}

CommonDataManager::CommonDataManager(QWidget* pMainWindow):
    m_dxfPath(""),
    m_mainWindow(pMainWindow)
{
}

CommonDataManager::~CommonDataManager() 
{

}


