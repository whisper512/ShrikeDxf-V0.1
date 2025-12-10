#pragma once
#ifndef DATAMANGER_H
#define DATAMANGER_H

#include <QPalette>


//管理通用数据

enum eThremColor
{
	eThremColor_Light,
	eThremColor_Dark
};

class CCommonDataManager
{
public:
	CCommonDataManager();
	~CCommonDataManager();

	QPalette* getPaletteLight(eThremColor color);

private:
	QPalette m_paletteLight , m_paletteDark;

private:
	void InitPalette();
};



#endif // DATAMANGER_H