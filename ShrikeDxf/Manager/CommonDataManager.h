#pragma once
#ifndef DATAMANGER_H
#define DATAMANGER_H

#include <QPalette>
#include <QString.h>

#include "../Widget/StyleSheet.h"

//通用管理类

class CommonDataManager
{
public:
	CommonDataManager();
	CommonDataManager(QWidget* pMainWindow);
	~CommonDataManager();

public:
	QString m_dxfPath;
	QWidget* m_mainWindow;

public:
	//静态成员函数

	//获取浅色主题样式表
	static const QPalette& getPaletteLight()
	{
		static const QPalette paletteLight = StyleSheet::createPaletteLigth();
		return paletteLight;
	};
	//获取深色主题样式表
	static const QPalette& getPaletteDark()
	{
		static const QPalette paletteDark = StyleSheet::createPaletteDark();
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