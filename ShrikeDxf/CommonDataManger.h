#pragma once
#ifndef DATAMANGER_H
#define DATAMANGER_H

#include <QPalette>
#include <QString.h>

#include "StyleSheet.h"

//管理通用数据


class CCommonDataManager
{
public:
	CCommonDataManager();
	~CCommonDataManager();

public:
	QString m_strDxfPath;


public:
	//静态成员函数

	//获取浅色主题样式表
	static const QPalette& getPaletteLight()
	{
		static const QPalette paletteLight = StyleSheet::CreatePaletteLigth();
		return paletteLight;
	};
	//获取深色主题样式表
	static const QPalette& getPaletteDark()
	{
		static const QPalette paletteDark = StyleSheet::CreatePaletteDark();
		return paletteDark;
	};
	//Qstring转std::string
	static const std::string QStringToStdString(const QString& str)
	{
		QByteArray ba = str.toLocal8Bit();
		const char* cstr = ba.constData();
		return std::string(cstr);
	};

private:
	
};



#endif // DATAMANGER_H