////获取variant中存储的数据和类型
//enumEntity GetVariantDxfEntity(variantDxfEntity dxfEntity, Point& point, Line& line, Circle& circle, Arc& arc, Polyline& polyline);
////获取entity数据
//QString GetEntityInfo(QString strLayer, QString strType, QString strNum);
////获取特定的entity
//variantDxfEntity GetEntity(QString strLayer, QString strType, QString strNum);
////保存选择的entity
//void SaveSelectedEntity(QString strLayer, QString strType, QString strNum);
////删除entity
//int DeleteEntity(QString strLayer, QString strType, QString strNum);
////粘贴entity
//int PasteEntity(QPointF pos);
//
////修改图层属性
//void ChangeLayerProperty();
////移动图元
//void MoveUpSelectedEntity();
//void MoveDownSelectedEntity();
//void MoveLeftSelectedEntity();
//void MoveRightSelectedEntity();
//void RotateCWSelectedEntity();
//void RotateCCWSelectedEntity();
////修改选中point属性
//void ChangePointProperty(Point point);
////修改选中line属性
//void ChangeLineProperty(Line line);
////修改选中circle属性
//void ChangeCircleProperty(Circle circle);
////修改选中arc属性
//void ChangeArcProperty(Arc arc);
////修改选中polyline属性
//void ChangePolylineProperty(Polyline polyline);


//void addPointToLayer(Point point, QString strLayer);
//void addLineToLayer(Line line, QString strLayer);
//void addCircleToLayer(Circle circle, QString strLayer);
//void addArcToLayer(Arc arc, QString strLayer);
//void addPolylineToLayer(QVector<QPointF>& m_vecPoints, QString strLayer);

//清空数据
//void ClearDxfData();


//    // 存放dxf文件中所有的数据
//    // 图层名 图层数据
//map<string, stuLayer> m_mapDxfEntities;
////tree当前选择的entity
//stuSelectedEntity m_SelectedEntity;
//
////移动的步长
//double m_dMoveStep = 1.0;
//double m_dRotateStepRAD = qDegreesToRadians(1.0);





//enumEntity CDxfReader::GetVariantDxfEntity(variantDxfEntity dxfEntity, Point& point, Line& line, Circle& circle, Arc& arc, Polyline& polyline)
//{
//	enumEntity EntityType = enumEntity_None;
//	std::visit([&](auto&& arg) {
//		using T = std::decay_t<decltype(arg)>;
//		if constexpr (std::is_same_v<T, Point>) {
//			point = arg;
//			EntityType = enumEntity_Point;
//		}
//		else if constexpr (std::is_same_v<T, Line>) {
//			line = arg;
//			EntityType = enumEntity_Line;
//		}
//		else if constexpr (std::is_same_v<T, Circle>) {
//			circle = arg;
//			EntityType = enumEntity_Circle;
//		}
//		else if constexpr (std::is_same_v<T, Arc>) {
//			arc = arg;
//			EntityType = enumEntity_Arc;
//		}
//		else if constexpr (std::is_same_v<T, Polyline>) {
//			polyline = arg;
//			EntityType = enumEntity_Polyline;
//		}
//		}, dxfEntity);
//	return EntityType;
//}
//
//
//void CDxfReader::ClearDxfData()
//{
//	if (!m_mapDxfEntities.empty())
//	{
//		m_mapDxfEntities.clear();
//	}
//}
//
//QString CDxfReader::GetEntityInfo(QString strLayer, QString strType, QString strNum)
//{
//	QString strInfo;
//	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		//获取图层内的具体图元信息
//		if (strType == STR_POINT_LOWERCASE)
//		{
//			Point point = CurLayer->second.vecPoints.at(strNum.toInt() - 1);
//			strInfo = QString("point%1\nx:%2,y:%3,z:%4").arg(strNum).arg(point.x()).arg(point.y()).arg(point.z);
//		}
//		else if (strType == STR_LINE_LOWERCASE)
//		{
//			Line line = CurLayer->second.vecLines.at(strNum.toInt() - 1);
//			strInfo = QString("line%1\nStartPoint  x:%2,y:%3,z:%4\nEndPoint - x:%5,y:%6,z:%7  ").
//				arg(strNum).arg(line.StartX()).arg(line.StartY()).arg(line.StartZ()).
//				arg(line.EndX()).arg(line.EndY()).arg(line.EndZ());
//		}
//		else if (strType == STR_CIRCLE_LOWERCASE)
//		{
//			Circle cycle = CurLayer->second.vecCircles.at(strNum.toInt() - 1);
//			strInfo = QString("circle%1\nCenterPoint  x:%2,y:%3,z:%4\nRadius - %5  ").
//				arg(strNum).arg(cycle.pointCenter.x()).arg(cycle.pointCenter.y()).arg(cycle.pointCenter.z).arg(cycle.radius);
//		}
//		else if (strType == STR_ARC_LOWERCASE)
//		{
//			Arc arc = CurLayer->second.vecArcs.at(strNum.toInt() - 1);
//			strInfo = QString("arc%1\nCenterPoint  x:%2,y:%3,z:%4\nRadius - %5\nStartAngle)%6\nEndAngle - %7  ").
//				arg(strNum).arg(arc.pointCenter.x()).arg(arc.pointCenter.y()).arg(arc.pointCenter.z).arg(arc.radius).
//				arg(arc.startAngle).arg(arc.endAngle);
//		}
//		else if (strType == STR_POLYLINE_LOWERCASE)
//		{
//			const Polyline& polyline = CurLayer->second.vecPolylines.at(strNum.toInt() - 1);
//			QString strPointsPos;
//			for (auto it = polyline.vecVertices.begin(); it != polyline.vecVertices.end(); it++)
//			{
//				strPointsPos += QString("x:%1,y:%2,z:%3\n").arg(it->x()).arg(it->y()).arg(it->z);
//			}
//			strInfo = QString("polyline%1\nnumVertices%2\n%3").arg(strNum).arg(polyline.numVertices).arg(strPointsPos);
//
//		}
//		else if (strType == STR_TEXT_LOWERCASE)
//		{
//			Text text = CurLayer->second.vecTexts.at(strNum.toInt() - 1);
//		}
//	}
//
//	return strInfo;
//}
//
//
//
//variantDxfEntity CDxfReader::GetEntity(QString strLayer, QString strType, QString strNum)
//{
//	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
//
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		//获取图层内的具体图元信息
//		if (strType == STR_POINT_LOWERCASE)
//		{
//			return CurLayer->second.vecPoints.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_LINE_LOWERCASE)
//		{
//			return CurLayer->second.vecLines.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_CIRCLE_LOWERCASE)
//		{
//			return CurLayer->second.vecCircles.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_ARC_LOWERCASE)
//		{
//			return CurLayer->second.vecArcs.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_POLYLINE_LOWERCASE)
//		{
//			return CurLayer->second.vecPolylines.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_TEXT_LOWERCASE)
//		{
//			return CurLayer->second.vecTexts.at(strNum.toInt() - 1);
//		}
//	}
//	return variantDxfEntity();
//}
//
//void CDxfReader::SaveSelectedEntity(QString strLayer, QString strType, QString strNum)
//{
//	m_SelectedEntity.strLayer = strLayer;
//	m_SelectedEntity.index = strNum.toInt() - 1;
//	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
//
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		//获取图层内的具体图元信息
//		if (strType == STR_POINT_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Point;
//			m_SelectedEntity.entity = CurLayer->second.vecPoints.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_LINE_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Line;
//			m_SelectedEntity.entity = CurLayer->second.vecLines.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_CIRCLE_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Circle;
//			m_SelectedEntity.entity = CurLayer->second.vecCircles.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_ARC_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Arc;
//			m_SelectedEntity.entity = CurLayer->second.vecArcs.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_POLYLINE_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Polyline;
//			m_SelectedEntity.entity = CurLayer->second.vecPolylines.at(strNum.toInt() - 1);
//		}
//		else if (strType == STR_TEXT_LOWERCASE)
//		{
//			m_SelectedEntity.type = enumEntity_Text;
//			m_SelectedEntity.entity = CurLayer->second.vecTexts.at(strNum.toInt() - 1);
//		}
//	}
//}
//
//int CDxfReader::DeleteEntity(QString strLayer, QString strType, QString strNum)
//{
//	auto CurLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		if (strType == STR_POINT_LOWERCASE)
//		{
//			CurLayer->second.vecPoints.erase(CurLayer->second.vecPoints.begin() + strNum.toInt() - 1);
//		}
//		else if (strType == STR_LINE_LOWERCASE)
//		{
//			CurLayer->second.vecLines.erase(CurLayer->second.vecLines.begin() + strNum.toInt() - 1);
//		}
//		else if (strType == STR_CIRCLE_LOWERCASE)
//		{
//			CurLayer->second.vecCircles.erase(CurLayer->second.vecCircles.begin() + strNum.toInt() - 1);
//		}
//		else if (strType == STR_ARC_LOWERCASE)
//		{
//			CurLayer->second.vecArcs.erase(CurLayer->second.vecArcs.begin() + strNum.toInt() - 1);
//		}
//		else if (strType == STR_POLYLINE_LOWERCASE)
//		{
//			CurLayer->second.vecPolylines.erase(CurLayer->second.vecPolylines.begin() + strNum.toInt() - 1);
//		}
//	}
//	return 0;
//}
//
//
//int CDxfReader::PasteEntity(QPointF pos)
//{
//	Point point;
//	Line line, newLine;
//	Circle circle;
//	Arc arc;
//	Polyline polyline, newPolyline;
//	enumEntity enumEntityType;
//
//
//	if (m_SelectedEntity.entity.index() != std::variant_npos)
//	{
//		enumEntityType = GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//
//		auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//		if (CurLayer != m_mapDxfEntities.end())
//		{
//			switch (enumEntityType)
//			{
//			case enumEntity_None:
//				break;
//			case enumEntity_Point:
//				point.setX(pos.x());
//				point.setY(pos.y());
//				CurLayer->second.vecPoints.push_back(point);
//				break;
//			case enumEntity_Line:
//			{
//				double lineMidX, lineMidY;
//				double offsetX, offsetY;
//				lineMidX = (line.EndX() + line.StartX()) / 2;
//				lineMidY = (line.EndY() + line.StartY()) / 2;
//				offsetX = pos.x() - lineMidX;
//				offsetY = pos.y() - lineMidY;
//				newLine.setStartPoint(Point(line.StartX() + offsetX, line.StartY() + offsetY));
//				newLine.setEndPoint(Point(line.EndX() + offsetX, line.EndY() + offsetY));
//				CurLayer->second.vecLines.push_back(newLine);
//				break;
//			}
//			case enumEntity_Circle:
//				circle.pointCenter.setX(pos.x());
//				circle.pointCenter.setY(pos.y());
//				CurLayer->second.vecCircles.push_back(circle);
//				break;
//			case enumEntity_Arc:
//				arc.pointCenter.setX(pos.x());
//				arc.pointCenter.setY(pos.y());
//				CurLayer->second.vecArcs.push_back(arc);
//				break;
//			case enumEntity_Polyline:
//			{
//				double sumX = 0, sumY = 0;
//				double centerX, centerY;
//				double offsetX, offsetY;
//				Point newVertex;
//				for (const auto& vertex : polyline.vecVertices)
//				{
//					sumX += vertex.x();
//					sumY += vertex.y();
//				}
//				centerX = sumX / polyline.vecVertices.size();
//				centerY = sumY / polyline.vecVertices.size();
//				offsetX = pos.x() - centerX;
//				offsetY = pos.y() - centerY;
//
//				newPolyline.numVertices = polyline.numVertices;
//				newPolyline.numVertices_M = polyline.numVertices_M;
//				newPolyline.numVertices_N = polyline.numVertices_N;
//				newPolyline.flag = polyline.flag;
//				// 平移所有顶点
//				for (const auto& vertex : polyline.vecVertices) {
//					newVertex.setX(newVertex.x() + offsetX);
//					newVertex.setY(newVertex.y() + offsetY);
//					newVertex.z = vertex.z;
//					newPolyline.vecVertices.push_back(newVertex);
//				}
//				CurLayer->second.vecPolylines.push_back(newPolyline);
//				break;
//			}
//			case enumEntity_Text:
//				break;
//			default:
//				break;
//			}
//		}
//		return 1;
//	}
//	else
//	{
//		return -1;
//	}
//}
//
//void CDxfReader::ChangeLayerProperty()
//{
//
//}
//
//void CDxfReader::MoveUpSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					//根据select的index找到对应的点修改dxf数据
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					point.setY(point.y() + m_dMoveStep);
//					m_SelectedEntity.entity = point;
//					CurLayer->second.vecPoints.at(m_SelectedEntity.index) = point;
//				}
//			}
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					line.setStartPoint(Point(line.StartX(), line.StartY() + m_dMoveStep));
//					line.setEndPoint(Point(line.EndX(), line.EndY() + m_dMoveStep));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					circle.pointCenter.setY(circle.pointCenter.y() + m_dMoveStep);
//					m_SelectedEntity.entity = circle;
//					CurLayer->second.vecCircles.at(m_SelectedEntity.index) = circle;
//				}
//			}
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.pointCenter.setY(arc.pointCenter.y() + m_dMoveStep);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					for (int i = 0; i < polyline.vecVertices.size(); i++)
//					{
//						polyline.vecVertices[i].setY(polyline.vecVertices[i].y() + m_dMoveStep);
//					}
//					m_SelectedEntity.entity = polyline;
//					CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//				}
//			}
//			break;
//		}
//
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::MoveDownSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					point.setY(point.y() - m_dMoveStep);
//					m_SelectedEntity.entity = point;
//					CurLayer->second.vecPoints.at(m_SelectedEntity.index) = point;
//				}
//			}
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					line.setStartPoint(Point(line.StartX(), line.StartY() - m_dMoveStep));
//					line.setEndPoint(Point(line.EndX(), line.EndY() - m_dMoveStep));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					circle.pointCenter.setY(circle.pointCenter.y() - m_dMoveStep);
//					m_SelectedEntity.entity = circle;
//					CurLayer->second.vecCircles.at(m_SelectedEntity.index) = circle;
//				}
//			}
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.pointCenter.setY(arc.pointCenter.y() - m_dMoveStep);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					for (int i = 0; i < polyline.vecVertices.size(); i++)
//					{
//						polyline.vecVertices[i].setY(polyline.vecVertices[i].y() - m_dMoveStep);
//					}
//					m_SelectedEntity.entity = polyline;
//					CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//				}
//			}
//			break;
//		}
//
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::MoveLeftSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					point.setX(point.x() - m_dMoveStep);
//					m_SelectedEntity.entity = point;
//					CurLayer->second.vecPoints.at(m_SelectedEntity.index) = point;
//				}
//			}
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					line.setStartPoint(Point(line.StartX() - m_dMoveStep, line.StartY()));
//					line.setEndPoint(Point(line.EndX() - m_dMoveStep, line.EndY()));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					circle.pointCenter.setX(circle.pointCenter.x() - m_dMoveStep);
//					m_SelectedEntity.entity = circle;
//					CurLayer->second.vecCircles.at(m_SelectedEntity.index) = circle;
//				}
//			}
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.pointCenter.setX(arc.pointCenter.x() - m_dMoveStep);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					for (int i = 0; i < polyline.vecVertices.size(); i++)
//					{
//						polyline.vecVertices[i].setX(polyline.vecVertices[i].x() - m_dMoveStep);
//					}
//					m_SelectedEntity.entity = polyline;
//					CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//				}
//			}
//			break;
//		}
//
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::MoveRightSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					point.setX(point.x() + m_dMoveStep);
//					m_SelectedEntity.entity = point;
//					CurLayer->second.vecPoints.at(m_SelectedEntity.index) = point;
//				}
//			}
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					line.setStartPoint(Point(line.StartX() + m_dMoveStep, line.StartY()));
//					line.setEndPoint(Point(line.EndX() + m_dMoveStep, line.EndY()));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					circle.pointCenter.setX(circle.pointCenter.x() + m_dMoveStep);
//					m_SelectedEntity.entity = circle;
//					CurLayer->second.vecCircles.at(m_SelectedEntity.index) = circle;
//				}
//			}
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.pointCenter.setX(arc.pointCenter.x() + m_dMoveStep);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					for (int i = 0; i < polyline.vecVertices.size(); i++)
//					{
//						polyline.vecVertices[i].setX(polyline.vecVertices[i].x() + m_dMoveStep);
//					}
//					m_SelectedEntity.entity = polyline;
//					CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//				}
//			}
//			break;
//		}
//
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::RotateCWSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					double endX = line.StartX() + line.Length() * qCos(line.Angle() - m_dRotateStepRAD);
//					double endY = line.StartY() + line.Length() * qSin(line.Angle() - m_dRotateStepRAD);
//
//					line.setStartPoint(Point(line.StartX(), line.StartY()));
//					line.setEndPoint(Point(endX, endY));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.startAngle -= qRadiansToDegrees(m_dRotateStepRAD);
//					arc.endAngle -= qRadiansToDegrees(m_dRotateStepRAD);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					if (!polyline.vecVertices.empty())
//					{
//						Point center = polyline.vecVertices[0];
//						double cosAngle = qCos(-m_dRotateStepRAD);
//						double sinAngle = qSin(-m_dRotateStepRAD);
//						for (int i = 0; i < polyline.vecVertices.size(); i++)
//						{
//							double dx = polyline.vecVertices[i].x() - center.x();
//							double dy = polyline.vecVertices[i].y() - center.y();
//							double newX = center.x() + dx * cosAngle - dy * sinAngle;
//							double newY = center.y() + dx * sinAngle + dy * cosAngle;
//							polyline.vecVertices[i].setX(newX);
//							polyline.vecVertices[i].setY(newY);
//						}
//						m_SelectedEntity.entity = polyline;
//						CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//					}
//				}
//			}
//			break;
//		}
//
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::RotateCCWSelectedEntity()
//{
//	Point point;
//	Line line;
//	Circle circle;
//	Arc arc;
//	Polyline polyline;
//	if (m_SelectedEntity.index != -1)
//	{
//		switch (m_SelectedEntity.type)
//		{
//		case enumEntity_Point:
//		{
//			break;
//		}
//		case enumEntity_Line:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					double endX = line.StartX() + line.Length() * qCos(line.Angle() + m_dRotateStepRAD);
//					double endY = line.StartY() + line.Length() * qSin(line.Angle() + m_dRotateStepRAD);
//
//					line.setStartPoint(Point(line.StartX(), line.StartY()));
//					line.setEndPoint(Point(endX, endY));
//					m_SelectedEntity.entity = line;
//					CurLayer->second.vecLines.at(m_SelectedEntity.index) = line;
//				}
//			}
//			break;
//		}
//		case enumEntity_Circle:
//		{
//			break;
//		}
//		case enumEntity_Arc:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//					arc.startAngle += qRadiansToDegrees(m_dRotateStepRAD);
//					arc.endAngle += qRadiansToDegrees(m_dRotateStepRAD);
//					m_SelectedEntity.entity = arc;
//					CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//				}
//			}
//			break;
//		}
//		case enumEntity_Polyline:
//		{
//			auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//			if (CurLayer != m_mapDxfEntities.end())
//			{
//				if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//				{
//					auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//					if (CurLayer != m_mapDxfEntities.end())
//					{
//						if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//						{
//							GetVariantDxfEntity(m_SelectedEntity.entity, point, line, circle, arc, polyline);
//							if (!polyline.vecVertices.empty())
//							{
//								Point center = polyline.vecVertices[0];
//								double cosAngle = qCos(m_dRotateStepRAD);
//								double sinAngle = qSin(m_dRotateStepRAD);
//								for (int i = 0; i < polyline.vecVertices.size(); i++)
//								{
//									double dx = polyline.vecVertices[i].x() - center.x();
//									double dy = polyline.vecVertices[i].y() - center.y();
//
//									double newX = center.x() + dx * cosAngle - dy * sinAngle;
//									double newY = center.y() + dx * sinAngle + dy * cosAngle;
//									polyline.vecVertices[i].setX(newX);
//									polyline.vecVertices[i].setY(newY);
//								}
//								m_SelectedEntity.entity = polyline;
//								CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//							}
//						}
//					}
//				}
//			}
//			break;
//		}
//		default:
//			break;
//		}
//	}
//}
//
//void CDxfReader::ChangePointProperty(Point point)
//{
//	auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		if (m_SelectedEntity.index <= CurLayer->second.vecPoints.size() && m_SelectedEntity.index >= 0)
//		{
//			//修改选中的图元
//			m_SelectedEntity.entity = point;
//			//修改dxf数据
//			CurLayer->second.vecPoints.at(m_SelectedEntity.index) = point;
//
//		}
//	}
//}
//
//void CDxfReader::ChangeLineProperty(Line line)
//{
//	auto Curlayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//	if (Curlayer != m_mapDxfEntities.end())
//	{
//		if (m_SelectedEntity.index <= Curlayer->second.vecLines.size() && m_SelectedEntity.index >= 0)
//		{
//			m_SelectedEntity.entity = line;
//			Curlayer->second.vecLines.at(m_SelectedEntity.index) = line;
//		}
//	}
//}
//
//void CDxfReader::ChangeCircleProperty(Circle circle)
//{
//	auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		if (m_SelectedEntity.index <= CurLayer->second.vecCircles.size() && m_SelectedEntity.index >= 0)
//		{
//			m_SelectedEntity.entity = circle;
//			CurLayer->second.vecCircles.at(m_SelectedEntity.index) = circle;
//		}
//	}
//}
//
//void CDxfReader::ChangeArcProperty(Arc arc)
//{
//	auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		if (m_SelectedEntity.index <= CurLayer->second.vecArcs.size() && m_SelectedEntity.index >= 0)
//		{
//			m_SelectedEntity.entity = arc;
//			CurLayer->second.vecArcs.at(m_SelectedEntity.index) = arc;
//		}
//	}
//}
//
//void CDxfReader::ChangePolylineProperty(Polyline polyline)
//{
//	auto CurLayer = m_mapDxfEntities.find(m_SelectedEntity.strLayer.toStdString());
//	if (CurLayer != m_mapDxfEntities.end())
//	{
//		if (m_SelectedEntity.index <= CurLayer->second.vecPolylines.size() && m_SelectedEntity.index >= 0)
//		{
//			m_SelectedEntity.entity = polyline;
//			CurLayer->second.vecPolylines.at(m_SelectedEntity.index) = polyline;
//		}
//	}
//}
//
//void CDxfReader::addPointToLayer(Point point, QString strLayer)
//{
//	auto curLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (curLayer != m_mapDxfEntities.end())
//	{
//		curLayer->second.vecPoints.push_back(point);
//	}
//}
//
//void CDxfReader::addLineToLayer(Line line, QString strLayer)
//{
//	auto curLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (curLayer != m_mapDxfEntities.end())
//	{
//		curLayer->second.vecLines.push_back(line);
//	}
//}
//
//void CDxfReader::addCircleToLayer(Circle circle, QString strLayer)
//{
//	auto curLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (curLayer != m_mapDxfEntities.end())
//	{
//		curLayer->second.vecCircles.push_back(circle);
//	}
//}
//
//void CDxfReader::addArcToLayer(Arc arc, QString strLayer)
//{
//	auto curLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (curLayer != m_mapDxfEntities.end())
//	{
//		curLayer->second.vecArcs.push_back(arc);
//	}
//}
//
//void CDxfReader::addPolylineToLayer(QVector<QPointF>& m_vecPoints, QString strLayer)
//{
//	if (m_vecPoints.isEmpty())
//	{
//		return;
//	}
//
//	Polyline polyline;
//	polyline.numVertices = m_vecPoints.size();
//	polyline.numVertices_M = 0;
//	polyline.numVertices_N = 0;
//	polyline.flag = 0; // 默认不闭合
//	for (const QPointF& point : m_vecPoints)
//	{
//		Point vertex(point.x(), point.y(), 0.0);
//		polyline.vecVertices.push_back(vertex);
//	}
//
//	auto curLayer = m_mapDxfEntities.find(strLayer.toStdString());
//	if (curLayer != m_mapDxfEntities.end())
//	{
//		curLayer->second.vecPolylines.push_back(polyline);
//	}
//
//}
