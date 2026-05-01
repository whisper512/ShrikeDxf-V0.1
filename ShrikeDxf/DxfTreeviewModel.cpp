#include "DxfTreeviewModel.h"

CDxfTreeviewModel::CDxfTreeviewModel(QObject* parent)
{
    m_iconPoint = QIcon(":/ShrikeDxf/res/Point.png");
    m_iconLine = QIcon(":/ShrikeDxf/res/Line.png");
    m_iconCircle = QIcon(":/ShrikeDxf/res/Circle.png");
    m_iconArc = QIcon(":/ShrikeDxf/res/Arc.png");
    m_iconPloyline = QIcon(":/ShrikeDxf/res/Polyline.png");
    m_iconLayer = QIcon(":/ShrikeDxf/res/Layer.png");
    m_iconEllipse = QIcon(":/ShrikeDxf/res/Ellipse.png");
    m_iconSolid = QIcon(":/ShrikeDxf/res/Hatch.png");
    m_iconSpline = QIcon(":/ShrikeDxf/res/Spline.png");
    m_iconText = QIcon(":/ShrikeDxf/res/Text.png");
    m_iconHatch = QIcon(":/ShrikeDxf/res/Hatch.png");

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
        //图层
        QList<QStandardItem*> listItemLayer;
        QStandardItem* itemLayer = new QStandardItem(QString::fromStdString(it->first));
        itemLayer->setIcon(m_iconLayer);
        QStandardItem* itemLayerPlaceHolder = new QStandardItem("");
        itemLayerPlaceHolder->setFlags(itemLayerPlaceHolder->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        listItemLayer << itemLayer << itemLayerPlaceHolder;
        appendRow(listItemLayer);
        // 图元索引
        int indexPoint = 1, indexLine = 1, indexCircle = 1, indexArc = 1, indexPolyline = 1, indexHatch = 1,
            indexText = 1, indexEllipse = 1, indexSpline = 1, indexMText = 1, indexSolid = 1 ,indexInsert = 1;
        // 图元索引
        int entityIndex = 0;
        
        //遍历图层内图元
        for (const auto& entity : it->second.entities)
        {
            EntityType type = GetEntityType(entity);
            QString qstr;
            QIcon icon;
            int* pIndex = nullptr;
            switch (type)
            {
            case EntityType::Point:
                qstr = QString("Point %1").arg(indexPoint++);
                icon = m_iconPoint;
                pIndex = &indexPoint;
                break;
            case EntityType::Line:
                qstr = QString("Line %1").arg(indexLine++);
                icon = m_iconLine;
                pIndex = &indexLine;
                break;
            case EntityType::Circle:
                qstr = QString("Circle %1").arg(indexCircle++);
                icon = m_iconCircle;
                pIndex = &indexCircle;
                break;
            case EntityType::Arc:
                qstr = QString("Arc %1").arg(indexArc++);
                icon = m_iconArc;
                pIndex = &indexArc;
                break;
            case EntityType::LWPolyline:
            case EntityType::Polyline:
                qstr = QString("Polyline %1").arg(indexPolyline++);
                icon = m_iconPloyline;
                pIndex = &indexPolyline;
                break;
            case EntityType::Ellipse:
                qstr = QString("Ellipse %1").arg(indexEllipse++);
                icon = m_iconEllipse;
                pIndex = &indexEllipse;
                break;
            case EntityType::Spline:
                qstr = QString("Spline %1").arg(indexSpline++);
                icon = m_iconSpline;
                pIndex = &indexSpline;
                break;
            case EntityType::Text:
                qstr = QString("Text %1").arg(indexText++);
                icon = m_iconText;
                pIndex = &indexText;
                break;
            case EntityType::MText:
                qstr = QString("MText %1").arg(indexMText++);
                icon = m_iconText;
                pIndex = &indexMText;
                break;
            case EntityType::Insert:
                qstr = QString("Insert %1").arg(indexInsert++);
                icon = m_iconLayer;
                pIndex = &indexInsert;
                break;
            case EntityType::Solid:
                qstr = QString("Solid %1").arg(indexSolid++);
                icon = m_iconHatch;
                pIndex = &indexSolid;
                break;
            case EntityType::Hatch:
                qstr = QString("Hatch %1").arg(indexHatch++);
                icon = m_iconHatch;
                pIndex = &indexHatch;
                break;
            default:
                qstr = QString("Unknown");
                break;
            }
            // 第二列显示图元名
            QList<QStandardItem*> listItemEnt;
            QStandardItem* itemPlaceHolder = new QStandardItem("");
            itemPlaceHolder->setFlags(itemPlaceHolder->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            QStandardItem* itemEnt = new QStandardItem(qstr);
            itemEnt->setIcon(icon);
            itemEnt->setData(entityIndex, Qt::UserRole);
            listItemEnt << itemPlaceHolder << itemEnt;
            itemLayer->appendRow(listItemEnt);
            entityIndex++;
        }
    }
}

void CDxfTreeviewModel::ClearModel()
{
    clear();
}


