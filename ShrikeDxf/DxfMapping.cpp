#include "DxfMapping.h"


CDxfMapping::CDxfMapping()
{
	

}

CDxfMapping::~CDxfMapping()
{
	
}

void CDxfMapping::addLayer(const DL_LayerData& data)
{
	ClearDxfData();
	//添加图层名作为键
	m_mapDxfEntities[data.name];
}

void CDxfMapping::addPoint(const DL_PointData& data)
{
	Point point{ data.x, data.y, data.z };

	std::string strCurrentLayer = getAttributes().getLayer();

	 auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if(CurLayer != m_mapDxfEntities.end())
	{
		CurLayer->second.vecPoints.push_back(point);
	}
}

void CDxfMapping::addLine(const DL_LineData& data)
{
	Point pStart{ data.x1, data.y1, data.z1 };
    Point pEnd{ data.x2, data.y2, data.z2 };
    Line line{ pStart, pEnd };

	std::string strCurrentLayer = getAttributes().getLayer();
	auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if (CurLayer != m_mapDxfEntities.end())
	{
		CurLayer->second.vecLines.push_back(line);
	}

}

void CDxfMapping::addCircle(const DL_CircleData& data)
{
	Point point{ data.cx, data.cy, data.cz };
	Circle cycle{point, data.radius };
	
	std::string strCurrentLayer = getAttributes().getLayer();
	auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if (CurLayer != m_mapDxfEntities.end())
	{
        CurLayer->second.vecCircles.push_back(cycle);
	}
}

void CDxfMapping::addArc(const DL_ArcData& data)
{
	Point point{ data.cx, data.cy, data.cz };
	Arc arc{ point, data.radius, data.angle1, data.angle2 };
	
	std::string strCurrentLayer = getAttributes().getLayer();
    auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if (CurLayer != m_mapDxfEntities.end())
	{
        CurLayer->second.vecArcs.push_back(arc);
	}
}

void CDxfMapping::addPolyline(const DL_PolylineData& data)
{
    Polyline Polyline;
	Polyline.numVertices = data.number;
	Polyline.numVertices_M = data.m;
	Polyline.numVertices_N = data.n;
	Polyline.flag = data.flags;

	std::string strCurrentLayer = getAttributes().getLayer();
    auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if (CurLayer != m_mapDxfEntities.end())
	{
        CurLayer->second.vecPolylines.push_back(Polyline);
		m_pCurPolyline = &CurLayer->second.vecPolylines.back();
		
	}
}

void CDxfMapping::addVertex(const DL_VertexData& data)
{
	if (m_pCurPolyline)
	{
		Point point{ data.x, data.y, data.z };
		m_pCurPolyline->vecVertices.push_back(point);
	}
}

void CDxfMapping::addText(const DL_TextData& data)
{
	//text 数据结构存在疑惑点, 没有读取到，暂时忽略
	Point point{ data.ipx, data.ipy, data.ipz };
	Text text;
	text.pointCenter = point;
    text.content = data.text;
    text.height = data.height;
}

void CDxfMapping::ClearDxfData()
{
	if (!m_mapDxfEntities.empty())
	{
		m_mapDxfEntities.clear();
	}
}