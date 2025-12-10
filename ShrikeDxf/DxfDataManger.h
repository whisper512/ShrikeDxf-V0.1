#pragma once
#ifndef DXF_DATA_MANGER_H
#define DXF_DATA_MANGER_H

#include <QString>

class CDxfDataManger
{
public:
	CDxfDataManger();
	~CDxfDataManger();

public:
	void SeterDxfFilePath(QString strPath);
	QString& GetDxfFilePath();
private:
	//dxfÂ·¾¶
	QString m_strDxfFilePath;
};




#endif //DXF_DATA_MANGER_H
