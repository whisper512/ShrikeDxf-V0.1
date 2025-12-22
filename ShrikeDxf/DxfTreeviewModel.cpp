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
    
    this->setHorizontalHeaderLabels(QStringList() << "FILE" << "LAYER" << "ENTITIES");

    QList<QStandardItem*> listRootItems;
    listRootItems << new QStandardItem("DXF") << new QStandardItem("") << new QStandardItem("");
    appendRow(listRootItems);
    
    QString qstr;

    //遍历图层
    for (auto it = layer.begin(); it != layer.end(); it++)
    {
        
        QList<QStandardItem*> listItemLayer;
        listItemLayer << new QStandardItem("") << new QStandardItem(QString::fromStdString(it->first)) << new QStandardItem("");
        listRootItems.first()->appendRow(listItemLayer);

        int indexPoint = 1,indexLine = 1, indexCircle = 1, indexArc = 1, indexPolyline = 1, indexText = 1;
        //遍历图层内的point
        for (auto itPoint = it->second.vecPoints.begin(); itPoint != it->second.vecPoints.end(); itPoint++)
        {
            qstr= QString("point%1").arg(indexPoint);
            QList<QStandardItem*> listItemPoint;
            listItemPoint << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemPoint);
            indexPoint++;
        }

        //遍历图层内的line
        for(auto itLine = it->second.vecLines.begin(); itLine != it->second.vecLines.end(); itLine++)
        {
            qstr = QString("line%1").arg(indexLine);
            QList<QStandardItem*> listItemLine;
            listItemLine << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemLine);
            indexLine++;
        }
        //遍历图层内的circle
        for (auto itCircle = it->second.vecCircles.begin(); itCircle != it->second.vecCircles.end(); itCircle++)
        {
            qstr = QString("circle%1").arg(indexCircle);
            QList<QStandardItem*> listItemCircle;
            listItemCircle << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemCircle);
            indexCircle++;
        }
        //遍历图层内的arc
        for (auto itArc = it->second.vecArcs.begin(); itArc != it->second.vecArcs.end(); itArc++)
        {
            qstr = QString("arc-%1").arg(indexArc);
            QList<QStandardItem*> listItemArc;
            listItemArc << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemArc);
            indexArc++;
        }
        //遍历图层内的polyline
        for (auto itPolyline = it->second.vecPolylines.begin(); itPolyline != it->second.vecPolylines.end(); itPolyline++)
        {
            qstr = QString("polyline-%1").arg(indexPolyline);
            QList<QStandardItem*> listItemPolyline;
            listItemPolyline << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemPolyline);
            indexPolyline++;
        }
        //遍历图层内的text
        for (auto itText = it->second.vecTexts.begin(); itText != it->second.vecTexts.end(); itText++)
        {
            qstr = QString("text-%1").arg(indexText);
            QList<QStandardItem*> listItemText;
            listItemText << new QStandardItem("") << new QStandardItem("") << new QStandardItem(qstr);
            listItemLayer.first()->appendRow(listItemText);
            indexText++;
        }
    }
}

void CDxfTreeviewModel::ClearModel()
{
    clear();
    m_layer.clear();
}


