#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) :
    m_pMainWnd(pMainWnd),
    m_strCurrentLayer("")
{
}

CDxfManger::~CDxfManger()
{
    delete m_pMainWnd;
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    DL_Dxf dxf;
    ClearDxfMappingData();
    if (!dxf.in(CCommonDataManager::QStringToStdString(strPath), &m_DxfMapping)) 
    {
        //打开文件失败
        QMessageBox::warning(nullptr, "Load File", "Open Dxf file failed");
        return false;
    }
    else
    {

        m_dxfVersion = dxf.getVersion();
        //更新model,通知treeview
        m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshTreeview(&m_DxfTreeviewModel);
        emit signalRefreshTreeviewAfterRead();

        //更新label
        emit signalShowFileName(strPath);

        //更新通知tableview

        m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);

        //绘制图形
        m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);

        //初始化步长
        emit signalSetStepLengthAndAngle(m_DxfMapping.m_dMoveStep, m_DxfMapping.m_dRotateStepRAD);

        return true;
    }

}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{
    // 创建DXF写入对象
    DL_Dxf* dxf = new DL_Dxf();

    // 创建DXF写入器
    DL_WriterA* dw = dxf->out(strPath.toStdString().c_str(), m_dxfVersion);
    if (!dw) 
    {
        delete dxf;
        QMessageBox::warning(nullptr, "Save File", "Failed to create DXF writer");
        return false;
    }

    // 写入DXF文件头部信息
    dxf->writeHeader(*dw);

    // 写入表部分
    dw->sectionTables();

    // 写入图层表
    dw->tableLayers(0);
    for (const auto& layerPair : m_DxfMapping.m_mapDxfEntities) {
        dxf->writeLayer(
            *dw,
            DL_LayerData(layerPair.first.c_str(), 0), // 图层名称和状态
            DL_Attributes(layerPair.first.c_str(),DxfColorMap::getColorIndex(layerPair.second.color),1,"CONTINUOUS",1.0)
        );
    }
    dw->tableEnd();

    // 写入其他表（线型、样式等）
    dw->sectionEnd();

    // 写入实体部分
    dw->sectionEntities();

    // 遍历所有图层和图元
    for (const auto& layerPair : m_DxfMapping.m_mapDxfEntities) 
    {
        const std::string& layerName = layerPair.first;
        const QColor& layerColor = layerPair.second.color;
        int colorIndex = DxfColorMap::getColorIndex(layerColor);

        // 写入点
        for (const auto& point : layerPair.second.vecPoints) 
        {
            dxf->writePoint(
                *dw,
                DL_PointData(point.x(), point.y(), 0.0),
                DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
        }

        // 写入线
        for (const auto& line : layerPair.second.vecLines) 
        {
            dxf->writeLine(
                *dw,
                DL_LineData(line.StartX(), line.StartY(), 0.0, line.EndX(), line.EndY(), 0.0),
                DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
        }

        // 写入圆
        for (const auto& circle : layerPair.second.vecCircles) {
            dxf->writeCircle(
                *dw,
                DL_CircleData(circle.pointCenter.x(), circle.pointCenter.y(), 0.0, circle.radius),
                DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
        }

        // 写入圆弧
        for (const auto& arc : layerPair.second.vecArcs) 
        {
            dxf->writeArc(
                *dw,
                DL_ArcData(arc.pointCenter.x(), arc.pointCenter.y(), 0.0, arc.radius, arc.startAngle, arc.endAngle),
                DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
        }

        // 写入多段线
        for (const auto& polyline : layerPair.second.vecPolylines) 
        {
            // 开始多段线
            dxf->writePolyline(
                *dw,
                DL_PolylineData(polyline.vecVertices.size(), 0, 0, 0),
                DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));

            // 写入多段线的顶点
            for (const auto& point : polyline.vecVertices) 
            {
                dxf->writeVertex(*dw,DL_VertexData(point.x(), point.y(), 0.0, 0.0));
            }

            // 结束多段线
            dxf->writePolylineEnd(*dw);
        }

        // 写入文本
        //for (const auto& text : layerPair.second.vecTexts) {
        //    dxf->writeText(
        //        *dw,
        //        DL_TextData(
        //            text.position.x, text.position.y, 0.0, // 插入点
        //            text.position.x, text.position.y, 0.0, // 对齐点
        //            text.height, // 文本高度
        //            1.0, // 宽度因子
        //            0, // 文本生成标志
        //            text.content.toStdString().c_str(), // 文本内容
        //            "STANDARD", // 文本样式
        //            0.0 // 旋转角度
        //        ),
        //        DL_Attributes(
        //            layerName.c_str(),
        //            colorIndex,
        //            1,
        //            "CONTINUOUS",
        //            1.0
        //        )
        //    );
        //}
    }

    // 结束实体部分
    dw->sectionEnd();

    // 结束DXF文件
    dw->close();

    // 清理资源
    delete dw;
    delete dxf;

    return true;
}

bool CDxfManger::NewDxfFile()
{
    //先关闭当前文件
    CloseDxfFile();
    
    //默认版本
    m_dxfVersion = DL_Codes::version::AC1015;
    //添加默认图层
    stuLayer layer;
    layer.color = QColor(0, 0, 0); //默认黑色
    m_DxfMapping.m_mapDxfEntities["0"] = layer;
    m_strCurrentLayer = "0";
    RefreshTreelAndGraphicsAndLayertable();
    return true;
}

bool CDxfManger::CloseDxfFile()
{
    ClearDxfMappingData();
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    RefreshTreelAndGraphicsAndLayertable();
    return false;
}

void CDxfManger::ClearDxfMappingData()
{
    m_DxfMapping.m_mapDxfEntities.clear();
}


QColor CDxfManger::GetCurrentLayerColor()
{
    if (m_DxfMapping.m_mapDxfEntities.find(m_strCurrentLayer.toStdString()) != m_DxfMapping.m_mapDxfEntities.end())
    {
        return m_DxfMapping.m_mapDxfEntities[m_strCurrentLayer.toStdString()].color;
    }
    return QColor();
}

QString CDxfManger::GetCurrentLayerName()
{
    return m_strCurrentLayer;
}

void CDxfManger::RefreshTreelAndGraphicsAndLayertable()
{
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
    m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
    emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);
}


QString CDxfManger::handleSaveSelectedEntity(const QString& strLayer, const QString& strEntity)
{
    QString strEntityData;

    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        //保存选择的图元信息
        m_DxfMapping.SaveSelectedEntity(strLayer, match.captured(1), match.captured(2));
        //通知stack刷新
        emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
        //通知MoveWidget
        emit signalSelectedEntityType(static_cast<int>(m_DxfMapping.m_SelectedEntity.type));
    }
    return strEntityData;
}

int CDxfManger::handleDeleteEntity(const QString& strLayer, const QString& strEntity)
{
    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        m_DxfMapping.DeleteEntity(strLayer, match.captured(1), match.captured(2));
        RefreshTreelAndGraphicsAndLayertable();
        return 1;
    }
    return -1;
}

int CDxfManger::handleCopyEntity()
{
    //通知graphics正在复制图元
    emit signalCopyintEntity();
    return 1;
}

void CDxfManger::handlePaste(QPointF pos)
{
    m_DxfMapping.PasteEntity(pos);
    RefreshTreelAndGraphicsAndLayertable();
}

void CDxfManger::handleLayerModelChanged()
{
    m_DxfLayerTableviewModel.UpdateLayerChangedData(m_DxfMapping.m_mapDxfEntities);
    RefreshTreelAndGraphicsAndLayertable();
}

void CDxfManger::handleChangeCurrentLayer(QString strLayer)
{
    m_strCurrentLayer = strLayer;
    emit signalCurrentLayer(strLayer);
}

void CDxfManger::handlePointAttributeChanged(Point point)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Point)
    {
        m_DxfMapping.ChangePointProperty(point);
        RefreshTreelAndGraphicsAndLayertable();
    }
}

void CDxfManger::handleLineAttributeChanged(Line line)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Line)
    {
        m_DxfMapping.ChangeLineProperty(line);
        RefreshTreelAndGraphicsAndLayertable();
    }
}

void CDxfManger::handleCircleAttributeChanged(Circle circle)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Circle)
    {
        m_DxfMapping.ChangeCircleProperty(circle);
        RefreshTreelAndGraphicsAndLayertable();
    }
}

void CDxfManger::handleArcAttributeChanged(Arc arc)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Arc)
    {
        m_DxfMapping.ChangeArcProperty(arc);
        RefreshTreelAndGraphicsAndLayertable();
    }
}

void CDxfManger::handlePolylineAttributeChanged(Polyline polyline)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Polyline)
    {
        m_DxfMapping.ChangePolylineProperty(polyline);
        RefreshTreelAndGraphicsAndLayertable();
    }
}

void CDxfManger::handleOnBtnUpClicked()
{
    m_DxfMapping.MoveUpSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);

}

void CDxfManger::handleOnBtnDownClicked()
{
    m_DxfMapping.MoveDownSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnLeftClicked()
{
    m_DxfMapping.MoveLeftSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnRightClicked()
{
    m_DxfMapping.MoveRightSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnCWClicked()
{
    m_DxfMapping.RotateCWSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnCCWClicked()
{
    m_DxfMapping.RotateCCWSelectedEntity();
    RefreshTreelAndGraphicsAndLayertable();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnStepLengthOrAngleChanged(double dStepLength, double dRotationAngle)
{
    m_DxfMapping.m_dMoveStep = dStepLength;
    //角度转弧度
    m_DxfMapping.m_dRotateStepRAD = dRotationAngle * (M_PI / 180);
}

void CDxfManger::handleMouseStatus(int index)
{
    m_DxfGraphicsScene.m_MouseState = (enumMouseStateInView)index;
    m_DxfGraphicsScene.clearPreviewEntityData();
    m_DxfGraphicsScene.ChangePreviewEntityByMouseState();
    //通知view预览绘图
    emit signalStartPreviewEntity(index);
}

void CDxfManger::handleGraphicsViewMouseMove(QPointF pos)
{
    m_DxfGraphicsScene.MouseMove(pos,GetCurrentLayerColor());
    //通知view刷新绘图
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
}

void CDxfManger::handleGraphicsViewLeftClick(QPointF pos)
{
    m_DxfGraphicsScene.MouseLeftClick(pos, GetCurrentLayerColor());
    switch (m_DxfGraphicsScene.m_PreviewEntity.type)
    {
    case enumEntity_Point:
    {
        //添加点
        Point point(pos.x(), pos.y());
        m_DxfMapping.addPointToLayer(point, GetCurrentLayerName());
        RefreshTreelAndGraphicsAndLayertable();
        break;
    }
    case enumPreviewEntity_Line:
    {
        m_DxfMapping.addLineToLayer(m_DxfGraphicsScene.m_linePreview,GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        m_DxfMapping.addCircleToLayer(m_DxfGraphicsScene.m_circlePreview, GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        m_DxfMapping.addCircleToLayer(m_DxfGraphicsScene.m_circlePreview, GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
    {
        if (m_DxfGraphicsScene.m_pointArcCenter.isNull() && m_DxfGraphicsScene.m_pointArcStart.isNull())
        {
            m_DxfMapping.addArcToLayer(m_DxfGraphicsScene.m_arcPreview, GetCurrentLayerName());
        }
        break;
    }
    default:
        break;
    }
    RefreshTreelAndGraphicsAndLayertable();
}

void CDxfManger::handleGraphicsViewRightClick(QPointF pos)
{
    m_DxfGraphicsScene.MouseRightClick(pos);
}

void CDxfManger::handleEndDrawPreview()
{
    //处理多段线
    if (m_DxfGraphicsScene.m_MouseState == enumMouseStateInView_PreviewPolyline && !m_DxfGraphicsScene.m_vecPolylinePoints.isEmpty())
    {
        m_DxfMapping.addPolylineToLayer(m_DxfGraphicsScene.m_vecPolylinePoints, GetCurrentLayerName());
        m_DxfGraphicsScene.m_vecPolylinePoints.clear();
    }

    m_DxfGraphicsScene.m_MouseState = enumMouseStateInView_None;
    //结束预览绘图,通知btn切换状态
    emit signalChangeCreateBtnStatus((int)m_DxfGraphicsScene.m_MouseState);
}