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
    
    this->setHorizontalHeaderLabels(QStringList() << "LAYER" << "ENTITIES");
    QString qstr;

    //遍历图层
    for (auto it = layer.begin(); it != layer.end(); it++)
    {
        
        QList<QStandardItem*> listItemLayer;
        QStandardItem* itemLayer = new QStandardItem(QString::fromStdString(it->first));
        QStandardItem* itemLayerBooth = new QStandardItem("");
        itemLayerBooth->setFlags(itemLayerBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        listItemLayer << itemLayer << itemLayerBooth;
        appendRow(listItemLayer);

        int indexPoint = 1,indexLine = 1, indexCircle = 1, indexArc = 1, indexPolyline = 1, indexText = 1;
        //遍历图层内的point
        for (auto itPoint = it->second.vecPoints.begin(); itPoint != it->second.vecPoints.end(); itPoint++)
        {
            qstr= QString("point%1").arg(indexPoint);
            QList<QStandardItem*> listItemPoint;
            QStandardItem* itemPoint = new QStandardItem(qstr);
            QStandardItem* itemPointBooth = new QStandardItem("");
            itemPointBooth->setFlags(itemPointBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemPoint << itemPointBooth << itemPoint;
            listItemLayer.first()->appendRow(listItemPoint);
            indexPoint++;
        }

        //遍历图层内的line
        for(auto itLine = it->second.vecLines.begin(); itLine != it->second.vecLines.end(); itLine++)
        {
            qstr = QString("line%1").arg(indexLine);
            QList<QStandardItem*> listItemLine;
            QStandardItem* itemLine = new QStandardItem(qstr);
            QStandardItem* itemLineBooth = new QStandardItem("");
            itemLineBooth->setFlags(itemLineBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemLine << itemLineBooth  << itemLine;
            listItemLayer.first()->appendRow(listItemLine);
            indexLine++;
        }
        //遍历图层内的circle
        for (auto itCircle = it->second.vecCircles.begin(); itCircle != it->second.vecCircles.end(); itCircle++)
        {
            qstr = QString("circle%1").arg(indexCircle);
            QList<QStandardItem*> listItemCircle;
            QStandardItem* itemCircle = new QStandardItem(qstr);
            QStandardItem* itemCircleBooth = new QStandardItem("");
            itemCircleBooth->setFlags(itemCircleBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemCircle << itemCircleBooth  << itemCircle;
            listItemLayer.first()->appendRow(listItemCircle);
            indexCircle++;
        }
        //遍历图层内的arc
        for (auto itArc = it->second.vecArcs.begin(); itArc != it->second.vecArcs.end(); itArc++)
        {
            qstr = QString("arc-%1").arg(indexArc);
            QList<QStandardItem*> listItemArc;
            QStandardItem* itemArc = new QStandardItem(qstr);
            QStandardItem* itemArcBooth = new QStandardItem("");
            itemArcBooth->setFlags(itemArcBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemArc << itemArcBooth  << itemArc;
            listItemLayer.first()->appendRow(listItemArc);
            indexArc++;
        }
        //遍历图层内的polyline
        for (auto itPolyline = it->second.vecPolylines.begin(); itPolyline != it->second.vecPolylines.end(); itPolyline++)
        {
            qstr = QString("polyline-%1").arg(indexPolyline);
            QList<QStandardItem*> listItemPolyline;
            QStandardItem* itemPolyline = new QStandardItem(qstr);
            QStandardItem* itemPolylineBooth = new QStandardItem("");
            itemPolylineBooth->setFlags(itemPolylineBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemPolyline << itemPolylineBooth  << itemPolyline;
            listItemLayer.first()->appendRow(listItemPolyline);
            indexPolyline++;
        }
        //遍历图层内的text
        for (auto itText = it->second.vecTexts.begin(); itText != it->second.vecTexts.end(); itText++)
        {
            qstr = QString("text-%1").arg(indexText);
            QList<QStandardItem*> listItemText;
            QStandardItem* itemText = new QStandardItem(qstr);
            QStandardItem* itemTextBooth = new QStandardItem("");
            itemTextBooth->setFlags(itemTextBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemText << itemTextBooth  << itemText;
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


