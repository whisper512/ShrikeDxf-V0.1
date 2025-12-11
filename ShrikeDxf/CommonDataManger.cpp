#include "CommonDataManger.h"

CCommonDataManager::CCommonDataManager():
    m_paletteLight("")
{
    InitPalette();
}

CCommonDataManager::~CCommonDataManager() 
{

}

void CCommonDataManager::InitPalette()
{  
    //°µÉ«
    m_paletteDark.setColor(QPalette::Window, QColor(53, 53, 53));
    m_paletteDark.setColor(QPalette::WindowText, Qt::white);
    m_paletteDark.setColor(QPalette::Base, QColor(25, 25, 25));
    m_paletteDark.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    m_paletteDark.setColor(QPalette::ToolTipBase, Qt::black);
    m_paletteDark.setColor(QPalette::ToolTipText, Qt::white);
    m_paletteDark.setColor(QPalette::Text, Qt::white);
    m_paletteDark.setColor(QPalette::Button, QColor(53, 53, 53));
    m_paletteDark.setColor(QPalette::ButtonText, Qt::white);
    m_paletteDark.setColor(QPalette::BrightText, Qt::red);
    m_paletteDark.setColor(QPalette::Link, QColor(42, 130, 218));
    m_paletteDark.setColor(QPalette::Highlight, QColor(42, 130, 218));
    m_paletteDark.setColor(QPalette::HighlightedText, Qt::black);

    //ÁÁÉ«
    m_paletteLight.setColor(QPalette::Window, QColor(240, 240, 240));
    m_paletteLight.setColor(QPalette::WindowText, Qt::black);
    m_paletteLight.setColor(QPalette::Base, QColor(255, 255, 255));
    m_paletteLight.setColor(QPalette::AlternateBase, QColor(233, 231, 227));
    m_paletteLight.setColor(QPalette::ToolTipBase, Qt::white);
    m_paletteLight.setColor(QPalette::ToolTipText, Qt::black);
    m_paletteLight.setColor(QPalette::Text, Qt::black);
    m_paletteLight.setColor(QPalette::Button, QColor(240, 240, 240));
    m_paletteLight.setColor(QPalette::ButtonText, Qt::black);
    m_paletteLight.setColor(QPalette::BrightText, Qt::red);
    m_paletteLight.setColor(QPalette::Link, QColor(0, 0, 255));
    m_paletteLight.setColor(QPalette::Highlight, QColor(0, 120, 215));
    m_paletteLight.setColor(QPalette::HighlightedText, Qt::white);
}

QPalette* CCommonDataManager::getPaletteLight(eThremColor color)
{
    if (color == eThremColor::eThremColor_Light)
    {
        return &m_paletteLight;
    }
    if (color == eThremColor::eThremColor_Dark)
    {
        return &m_paletteDark;
    }
    return nullptr;
}

