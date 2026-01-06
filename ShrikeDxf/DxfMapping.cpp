#include "DxfMapping.h"


CDxfMapping::CDxfMapping()
{
	

}

CDxfMapping::~CDxfMapping()
{
	
}

void CDxfMapping::addLayer(const DL_LayerData& data)
{

	if (m_mapDxfEntities.find(data.name) == m_mapDxfEntities.end())
	{
		//添加图层名作为键
		m_mapDxfEntities[data.name];
	}

	//添加图层颜色
	std::string strCurrentLayer = getAttributes().getLayer();
	auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	//初始化dxf颜色索引和颜色的对应
	const auto& mapDxfColor = DxfColorMap::getColorMap();
	if (CurLayer != m_mapDxfEntities.end())
	{
		CurLayer->second.color = mapDxfColor.at(getAttributes().getColor());
	}
	
}

void CDxfMapping::addPoint(const DL_PointData& data)
{
	Point point{ data.x, data.y, data.z };
	//获取当前的图层
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

	std::string strCurrentLayer = getAttributes().getLayer();
	auto CurLayer = m_mapDxfEntities.find(strCurrentLayer);
	if (CurLayer != m_mapDxfEntities.end())
	{
		CurLayer->second.vecTexts.push_back(text);
	}

}

void CDxfMapping::ClearDxfData()
{
	if (!m_mapDxfEntities.empty())
	{
		m_mapDxfEntities.clear();
	}
}

QString CDxfMapping::GetEntityInfo(QString strLayer, QString strType, QString strNum)
{
	QString strInfo;
	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
	if (CurLayer != m_mapDxfEntities.end())
	{
		//获取图层内的具体图元信息
		if (strType == STR_POINT_LOWERCASE)
		{
			Point point = CurLayer->second.vecPoints.at(strNum.toInt() - 1);
            strInfo = QString("point%1\nx:%2,y:%3,z:%4").arg(strNum).arg(point.x).arg(point.y).arg(point.z);
		}
		else if (strType == STR_LINE_LOWERCASE)
		{
			Line line = CurLayer->second.vecLines.at(strNum.toInt() - 1);
			strInfo = QString("line%1\nStartPoint  x:%2,y:%3,z:%4\nEndPoint - x:%5,y:%6,z:%7  ").
				arg(strNum).arg(line.pointStart.x).arg(line.pointStart.y).arg(line.pointStart.z).
				arg(line.pointEnd.x).arg(line.pointEnd.y).arg(line.pointEnd.z);
		}
		else if (strType == STR_CIRCLE_LOWERCASE)
		{
            Circle cycle = CurLayer->second.vecCircles.at(strNum.toInt() - 1);
			strInfo = QString("circle%1\nCenterPoint  x:%2,y:%3,z:%4\nRadius - %5  ").
				arg(strNum).arg(cycle.pointCenter.x).arg(cycle.pointCenter.y).arg(cycle.pointCenter.z).arg(cycle.radius);
		}
		else if (strType == STR_ARC_LOWERCASE)
		{
            Arc arc = CurLayer->second.vecArcs.at(strNum.toInt() - 1);
            strInfo = QString("arc%1\nCenterPoint  x:%2,y:%3,z:%4\nRadius - %5\nStartAngle)%6\nEndAngle - %7  ").
                arg(strNum).arg(arc.pointCenter.x).arg(arc.pointCenter.y).arg(arc.pointCenter.z).arg(arc.radius).
                arg(arc.startAngle).arg(arc.endAngle);
		}
		else if (strType == STR_POLYLINE_LOWERCASE)
		{
			const Polyline& polyline = CurLayer->second.vecPolylines.at(strNum.toInt() - 1);
			QString strPointsPos;
			for (auto it = polyline.vecVertices.begin(); it != polyline.vecVertices.end(); it++)
			{
				strPointsPos += QString("x:%1,y:%2,z:%3\n").arg(it->x).arg(it->y).arg(it->z);
			}
			strInfo = QString("polyline%1\nnumVertices%2\n%3").arg(strNum).arg(polyline.numVertices).arg(strPointsPos);

		}
		else if (strType == STR_TEXT_LOWERCASE)
		{
            Text text = CurLayer->second.vecTexts.at(strNum.toInt() - 1);
            strInfo = QString("text%1\nCenterPoint  x:%2,y:%3,z:%4\nContent - %5\nHeight - %6").
				arg(strNum).arg(text.pointCenter.x).arg(text.pointCenter.y).arg(text.pointCenter.z).arg(text.content).arg(text.height);
		}
	}
	
	return strInfo;
}

DxfEntity CDxfMapping::GetEntity(QString strLayer, QString strType, QString strNum)
{
	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
	if (CurLayer != m_mapDxfEntities.end())
	{
		//获取图层内的具体图元信息
		if (strType == STR_POINT_LOWERCASE)
		{
			return CurLayer->second.vecPoints.at(strNum.toInt() - 1);
		}
		else if (strType == STR_LINE_LOWERCASE)
		{
			return CurLayer->second.vecLines.at(strNum.toInt() - 1);
		}
		else if (strType == STR_CIRCLE_LOWERCASE)
		{
			return CurLayer->second.vecCircles.at(strNum.toInt() - 1);
		}
		else if (strType == STR_ARC_LOWERCASE)
		{
			return CurLayer->second.vecArcs.at(strNum.toInt() - 1);
		}
		else if (strType == STR_POLYLINE_LOWERCASE)
		{
			return CurLayer->second.vecPolylines.at(strNum.toInt() - 1);
		}
		else if (strType == STR_TEXT_LOWERCASE)
		{
			return CurLayer->second.vecTexts.at(strNum.toInt() - 1);
		}
	}
	return DxfEntity();
}
