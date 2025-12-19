#include "DxfTreeviewModel.h"

CDxfTreeviewModel::CDxfTreeviewModel(QObject* parent)
{
}

CDxfTreeviewModel::~CDxfTreeviewModel()
{
}

void CDxfTreeviewModel::UpdateLayoutItem(const std::map<std::string, stuLayer>& layer)
{

    ClearModel();
    
    //添加根节点
    QStandardItem* rootItem = new QStandardItem(QString("DXF"));
    appendRow(rootItem);
    
    QString qstr;

    //遍历图层
    for (auto it = layer.begin(); it != layer.end(); it++)
    {
        QStandardItem* item = new QStandardItem(QString::fromStdString(it->first));
        

        int indexPoint = 1,indexLine = 1, indexCircle = 1, indexArc = 1, indexPolyline = 1, indexText = 1;
        //遍历图层内的point
        for (auto itPoint = it->second.vecPoints.begin(); itPoint != it->second.vecPoints.end(); itPoint++)
        {
            qstr= QString("point-%1").arg(indexPoint);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexPoint++;
        }
        //遍历图层内的line
        for(auto itLine = it->second.vecLines.begin(); itLine != it->second.vecLines.end(); itLine++)
        {
            qstr = QString("line-%1").arg(indexLine);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexLine++;
        }
        //遍历图层内的circle
        for (auto itCircle = it->second.vecCircles.begin(); itCircle != it->second.vecCircles.end(); itCircle++)
        {
            qstr = QString("circle-%1").arg(indexCircle);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexCircle++;
        }
        //遍历图层内的arc
        for (auto itArc = it->second.vecArcs.begin(); itArc != it->second.vecArcs.end(); itArc++)
        {
            qstr = QString("arc-%1").arg(indexArc);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexArc++;
        }
        //遍历图层内的polyline
        for (auto itPolyline = it->second.vecPolylines.begin(); itPolyline != it->second.vecPolylines.end(); itPolyline++)
        {
            qstr = QString("polyline-%1").arg(indexPolyline);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexPolyline++;
        }
        //遍历图层内的text
        for (auto itText = it->second.vecTexts.begin(); itText != it->second.vecTexts.end(); itText++)
        {
            qstr = QString("text-%1").arg(indexText);
            QStandardItem* itemSub = new QStandardItem(qstr);
            item->appendRow(itemSub);
            indexText++;
        }

        rootItem->appendRow(item);
    }

}

void CDxfTreeviewModel::ClearModel()
{
    clear();
    m_layer.clear();
}


