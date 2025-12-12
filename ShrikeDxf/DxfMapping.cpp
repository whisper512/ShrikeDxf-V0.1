#include "DxfMapping.h"


CDxfMapping::CDxfMapping()
{
	m_vecPoints.clear();

}

CDxfMapping::~CDxfMapping()
{
	m_vecPoints.clear();
}

int CDxfMapping::GetPointCount()
{
	return m_vecPoints.size(); 
}

void CDxfMapping::addPoint(const DL_PointData& data)
{
	m_vecPoints.push_back(Point(data.x, data.y, data.z));
}

