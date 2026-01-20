#include "DxfTreeviewModel.h"

CDxfTreeviewModel::CDxfTreeviewModel(QObject* parent)
{
    m_iconPoint = QIcon(":/ShrikeDxf/res/Point.png");
    m_iconLine = QIcon(":/ShrikeDxf/res/Line.png");
    m_iconCircle = QIcon(":/ShrikeDxf/res/Circle.png");
    m_iconArc = QIcon(":/ShrikeDxf/res/Arc.png");
    m_iconPloyline = QIcon(":/ShrikeDxf/res/Polyline.png");
}

CDxfTreeviewModel::~CDxfTreeviewModel()
{
}

void CDxfTreeviewModel::UpdateLayoutItemModel(const std::map<std::string, stuLayer>& mapDxf)
{

    ClearModel();
    
    this->setHorizontalHeaderLabels(QStringList() << "LAYER" << "ENTITIES");
    QString qstr;

    //遍历图层
    for (auto it = mapDxf.begin(); it != mapDxf.end(); it++)
    {
        QList<QStandardItem*> listItemLayer;
        QStandardItem* itemLayer = new QStandardItem(QString::fromStdString(it->first));
        QStandardItem* itemLayerPlaceHolder = new QStandardItem("");
        itemLayerPlaceHolder->setFlags(itemLayerPlaceHolder->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        listItemLayer << itemLayer << itemLayerPlaceHolder;
        appendRow(listItemLayer);

        int indexPoint = 1,indexLine = 1, indexCircle = 1, indexArc = 1, indexPolyline = 1, indexText = 1;
        //遍历图层内的point
        for (auto itPoint = it->second.vecPoints.begin(); itPoint != it->second.vecPoints.end(); itPoint++)
        {
            qstr= QString("%1%2").arg(STR_POINT_LOWERCASE).arg(indexPoint);
            QList<QStandardItem*> listItemPoint;
            QStandardItem* itemPoint = new QStandardItem(qstr);
            itemPoint->setIcon(m_iconPoint);
            QStandardItem* itemPointPlaceHolder = new QStandardItem("");
            itemPointPlaceHolder->setFlags(itemPointPlaceHolder->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemPoint << itemPointPlaceHolder << itemPoint;
            listItemLayer.first()->appendRow(listItemPoint);
            indexPoint++;
        }

        //遍历图层内的line
        for(auto itLine = it->second.vecLines.begin(); itLine != it->second.vecLines.end(); itLine++)
        {
            qstr = QString("%1%2").arg(STR_LINE_LOWERCASE).arg(indexLine);
            QList<QStandardItem*> listItemLine;
            QStandardItem* itemLine = new QStandardItem(qstr);
            itemLine->setIcon(m_iconLine);
            QStandardItem* itemLineBooth = new QStandardItem("");
            itemLineBooth->setFlags(itemLineBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemLine << itemLineBooth  << itemLine;
            listItemLayer.first()->appendRow(listItemLine);
            indexLine++;
        }
        //遍历图层内的circle
        for (auto itCircle = it->second.vecCircles.begin(); itCircle != it->second.vecCircles.end(); itCircle++)
        {
            qstr = QString("%1%2").arg(STR_CIRCLE_LOWERCASE).arg(indexCircle);
            QList<QStandardItem*> listItemCircle;
            QStandardItem* itemCircle = new QStandardItem(qstr);
            itemCircle->setIcon(m_iconCircle);
            QStandardItem* itemCircleBooth = new QStandardItem("");
            itemCircleBooth->setFlags(itemCircleBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemCircle << itemCircleBooth  << itemCircle;
            listItemLayer.first()->appendRow(listItemCircle);
            indexCircle++;
        }
        //遍历图层内的arc
        for (auto itArc = it->second.vecArcs.begin(); itArc != it->second.vecArcs.end(); itArc++)
        {
            qstr = QString("%1%2").arg(STR_ARC_LOWERCASE).arg(indexArc);
            QList<QStandardItem*> listItemArc;
            QStandardItem* itemArc = new QStandardItem(qstr);
            itemArc->setIcon(m_iconArc);
            QStandardItem* itemArcBooth = new QStandardItem("");
            itemArcBooth->setFlags(itemArcBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemArc << itemArcBooth  << itemArc;
            listItemLayer.first()->appendRow(listItemArc);
            indexArc++;
        }
        //遍历图层内的polyline
        for (auto itPolyline = it->second.vecPolylines.begin(); itPolyline != it->second.vecPolylines.end(); itPolyline++)
        {
            qstr = QString("%1%2").arg(STR_POLYLINE_LOWERCASE).arg(indexPolyline);
            QList<QStandardItem*> listItemPolyline;
            QStandardItem* itemPolyline = new QStandardItem(qstr);
            itemPolyline->setIcon(m_iconPloyline);
            QStandardItem* itemPolylineBooth = new QStandardItem("");
            itemPolylineBooth->setFlags(itemPolylineBooth->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            listItemPolyline << itemPolylineBooth  << itemPolyline;
            listItemLayer.first()->appendRow(listItemPolyline);
            indexPolyline++;
        }
        //遍历图层内的text
        for (auto itText = it->second.vecTexts.begin(); itText != it->second.vecTexts.end(); itText++)
        {
            qstr = QString("%1%2").arg(STR_TEXT_LOWERCASE).arg(indexText);
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
    m_mapDxf.clear();
}


