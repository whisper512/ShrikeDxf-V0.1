#include "DxfDataManger.h"

CDxfDataManger::CDxfDataManger():
	m_strDxfFilePath()
{
}

CDxfDataManger::~CDxfDataManger()
{
}

void CDxfDataManger::SeterDxfFilePath(QString strPath)
{
	if (m_strDxfFilePath != strPath)
	{
        m_strDxfFilePath = strPath;
	}
}

QString& CDxfDataManger::GetDxfFilePath()
{
	return m_strDxfFilePath;
}
