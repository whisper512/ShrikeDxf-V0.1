#pragma once
#include "DxfMapping.h"


//dxf管理类,包含映射,操作，绘图
class CDxfManger
{
public:
	CDxfManger();
	~CDxfManger();

public:
	bool LoadDxfFile(const QString& strPath);
	int GetDxfPointCount();

private:
	//dxf映射类
	CDxfMapping m_DxfMapping;

};

