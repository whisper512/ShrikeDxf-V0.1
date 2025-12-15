#include "DxfMapping.h"


CDxfMapping::CDxfMapping()
{
	

}

CDxfMapping::~CDxfMapping()
{
	
}

void CDxfMapping::addPoint(const DL_PointData& data)
{
	Point point{ data.x, data.y, data.z };
	m_DxfStruct.addEntity(currentAttrib.getLayer(), point);
}

void CDxfMapping::addLine(const DL_LineData& data)
{
	Point pStart{ data.x1, data.y1, data.z1 };
    Point pEnd{ data.x2, data.y2, data.z2 };
    Line line{ pStart, pEnd };
	m_DxfStruct.addEntity(currentAttrib.getLayer(), line);
}

void CDxfMapping::addCircle(const DL_CircleData& data)
{
	Point point{ data.cx, data.cy, data.cz };
	Circle cycle{point, data.radius };
	m_DxfStruct.addEntity(currentAttrib.getLayer(), cycle);
}

void CDxfMapping::addArc(const DL_ArcData& data)
{
	Point point{ data.cx, data.cy, data.cz };
	Arc arc{ point, data.radius, data.angle1, data.angle2 };
	m_DxfStruct.addEntity(currentAttrib.getLayer(), arc);
}

void CDxfMapping::addPolyline(const DL_PolylineData& data)
{
	currentPolyline = Polyline{};
	currentPolyline.closed = (data.flags & 0x01) != 0;
	inPolyline = true;
}

void CDxfMapping::addVertex(const DL_VertexData& data)
{
	if (inPolyline) 
	{
		currentPolyline.vertices.push_back({ data.x, data.y, data.z });
	}
}

void CDxfMapping::endEntity()
{
	if (inPolyline && !currentPolyline.vertices.empty()) 
	{
		m_DxfStruct.addEntity(currentAttrib.getLayer(), currentPolyline);
		inPolyline = false;
	}
}

void CDxfMapping::addText(const DL_TextData& data)
{
	//text 数据结构存在疑惑点, 需要修正
	Point point{ data.ipx, data.ipy, data.ipz };
	Text text{ point, data.text,data.height };
	m_DxfStruct.addEntity(currentAttrib.getLayer(), text);
}
