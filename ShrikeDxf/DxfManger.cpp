#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
{
}

CDxfManger::~CDxfManger()
{
    delete m_pMainWnd;
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    //DL_Dxf dxf;
    //ClearDxfMappingData();
    //if (!dxf.in(CCommonDataManager::QStringToStdString(strPath), &m_DxfMapping)) 
    //{
    //    //打开文件失败
    //    QMessageBox::warning(nullptr, "Load File", "Open Dxf file failed");
    //    return false;
    //}
    //else
    //{

    //    //m_dxfVersion = dxf.getVersion();
    //    //更新model,通知treeview
    //    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
    //    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    //    emit signalRefreshTreeviewAfterRead();

    //    //更新label
    //    emit signalShowFileName(strPath);

    //    //更新通知tableview

    //    m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
    //    emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);

    //    //绘制图形
    //    m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
    //    emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);

    //    //初始化步长
    //    emit signalSetStepLengthAndAngle(m_DxfMapping.m_dMoveStep, m_DxfMapping.m_dRotateStepRAD);

    //    return true;
    //}
    return false;
}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{
    //// 创建DXF写入对象
    //DL_Dxf* dxf = new DL_Dxf();

    //// 创建DXF写入器
    //DL_WriterA* dw = dxf->out(strPath.toStdString().c_str(), m_dxfVersion);
    //if (!dw) 
    //{
    //    delete dxf;
    //    QMessageBox::warning(nullptr, "Save File", "Failed to create DXF writer");
    //    return false;
    //}

    //// 写入DXF文件头部信息
    //dxf->writeHeader(*dw);

    //// 写入表部分
    //dw->sectionTables();

    //// 写入图层表
    //dw->tableLayers(0);
    //for (const auto& layerPair : m_DxfMapping.m_mapDxfEntities) {
    //    dxf->writeLayer(
    //        *dw,
    //        DL_LayerData(layerPair.first.c_str(), 0), // 图层名称和状态
    //        DL_Attributes(layerPair.first.c_str(),DxfColorMap::getColorIndex(layerPair.second.color),1,"CONTINUOUS",1.0)
    //    );
    //}
    //dw->tableEnd();

    //// 写入其他表（线型、样式等）
    //dw->sectionEnd();

    //// 写入实体部分
    //dw->sectionEntities();

    //// 遍历所有图层和图元
    //for (const auto& layerPair : m_DxfMapping.m_mapDxfEntities) 
    //{
    //    const std::string& layerName = layerPair.first;
    //    const QColor& layerColor = layerPair.second.color;
    //    int colorIndex = DxfColorMap::getColorIndex(layerColor);

    //    // 写入点
    //    for (const auto& point : layerPair.second.vecPoints) 
    //    {
    //        dxf->writePoint(
    //            *dw,
    //            DL_PointData(point.x(), point.y(), 0.0),
    //            DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
    //    }

    //    // 写入线
    //    for (const auto& line : layerPair.second.vecLines) 
    //    {
    //        dxf->writeLine(
    //            *dw,
    //            DL_LineData(line.StartX(), line.StartY(), 0.0, line.EndX(), line.EndY(), 0.0),
    //            DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
    //    }

    //    // 写入圆
    //    for (const auto& circle : layerPair.second.vecCircles) {
    //        dxf->writeCircle(
    //            *dw,
    //            DL_CircleData(circle.pointCenter.x(), circle.pointCenter.y(), 0.0, circle.radius),
    //            DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
    //    }

    //    // 写入圆弧
    //    for (const auto& arc : layerPair.second.vecArcs) 
    //    {
    //        dxf->writeArc(
    //            *dw,
    //            DL_ArcData(arc.pointCenter.x(), arc.pointCenter.y(), 0.0, arc.radius, arc.startAngle, arc.endAngle),
    //            DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));
    //    }

    //    // 写入多段线
    //    for (const auto& polyline : layerPair.second.vecPolylines) 
    //    {
    //        // 开始多段线
    //        dxf->writePolyline(
    //            *dw,
    //            DL_PolylineData((int)polyline.vecVertices.size(), 0, 0, 0),
    //            DL_Attributes(layerName.c_str(),colorIndex,1,"CONTINUOUS",1.0));

    //        // 写入多段线的顶点
    //        for (const auto& point : polyline.vecVertices) 
    //        {
    //            dxf->writeVertex(*dw,DL_VertexData(point.x(), point.y(), 0.0, 0.0));
    //        }

    //        // 结束多段线
    //        dxf->writePolylineEnd(*dw);
    //    }

    //    // 写入文本
    //    //for (const auto& text : layerPair.second.vecTexts) {
    //    //    dxf->writeText(
    //    //        *dw,
    //    //        DL_TextData(
    //    //            text.position.x, text.position.y, 0.0, // 插入点
    //    //            text.position.x, text.position.y, 0.0, // 对齐点
    //    //            text.height, // 文本高度
    //    //            1.0, // 宽度因子
    //    //            0, // 文本生成标志
    //    //            text.content.toStdString().c_str(), // 文本内容
    //    //            "STANDARD", // 文本样式
    //    //            0.0 // 旋转角度
    //    //        ),
    //    //        DL_Attributes(
    //    //            layerName.c_str(),
    //    //            colorIndex,
    //    //            1,
    //    //            "CONTINUOUS",
    //    //            1.0
    //    //        )
    //    //    );
    //    //}
    //}

    //// 结束实体部分
    //dw->sectionEnd();

    //// 结束DXF文件
    //dw->close();

    //// 清理资源
    //delete dw;
    //delete dxf;

    return true;
}

bool CDxfManger::NewDxfFile()
{
    ////先关闭当前文件
    //CloseDxfFile();
    //
    ////默认版本
    //m_dxfVersion = DL_Codes::version::AC1015;
    ////添加默认图层
    //stuLayer layer;
    //layer.color = QColor(0, 0, 0); //默认黑色
    //m_DxfMapping.m_mapDxfEntities["0"] = layer;
    //m_strCurrentLayer = "0";
    //m_DxfGraphicsScene.setSceneRect(500, 500, 500, 500);
    //
    //m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
    //m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);

    //emit signalRefreshTreeview(&m_DxfTreeviewModel);
    //emit signalRefreshGraphicsview(&m_DxfGraphicsScene, true);
    //emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);
    //emit signalSetStepLengthAndAngle(m_DxfMapping.m_dMoveStep, m_DxfMapping.m_dRotateStepRAD);
    return true;
}

bool CDxfManger::CloseDxfFile()
{
    ClearDxfMappingData();
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}

void CDxfManger::ClearDxfMappingData()
{
    //m_DxfMapping.m_mapDxfEntities.clear();
}