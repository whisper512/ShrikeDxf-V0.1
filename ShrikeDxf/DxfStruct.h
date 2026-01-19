#pragma once
#ifndef DxfStruct_h
#define DxfStruct_h
#include <iostream>
#include <QString>
#include <vector>
#include <map>
#include <QColor>

#include "Primitive.h"

//存储dxf文件数据结构
using namespace std;

//dxf颜色索引和实际颜色
class DxfColorMap
{
public:
    static const std::map<int, QColor>& getColorMap()
    {
        static std::map<int, QColor> colorMap = []()
        {
            std::map<int, QColor> map;

            // 初始化标准颜色 (1-7)
            map[1] = Qt::red;        // 红色
            map[2] = Qt::yellow;     // 黄色
            map[3] = Qt::green;      // 绿色
            map[4] = Qt::cyan;       // 青色
            map[5] = Qt::blue;       // 蓝色
            map[6] = Qt::magenta;    // 洋红色
            map[7] = Qt::white;      // 白色/黑色（取决于背景色）

            // 初始化8-255的颜色
            for (int i = 8; i <= 255; ++i) 
            {
                int r, g, b;

                if (i <= 9) 
                {
                    int gray = 255 - (i - 8) * 50;
                    r = g = b = gray;
                }
                else if (i <= 249) 
                {
                    int colorIndex = i - 10;
                    int colorGroup = colorIndex / 16;
                    int indexInGroup = colorIndex % 16;

                    switch (colorGroup) 
                    {
                    case 0:  // 红色调
                        r = 255;
                        g = indexInGroup * 17;
                        b = 0;
                        break;
                        // ... 其他颜色组的处理逻辑 ...
                    default:
                        r = g = b = 128;
                        break;
                    }
                }
                else 
                {
                    int gray = 255 - (i - 250) * 40;
                    r = g = b = gray;
                }

                map[i] = QColor(r, g, b);
            }

            // 添加0号颜色（表示随块）
            map[0] = Qt::black;

            return map;
        }();
        return colorMap;
    }
};






//某个图元
using variantDxfEntity = std::variant<Point, Line, Circle, Arc, Polyline, Text>;

struct stuSelectedEntity
{
    enumEntity type;
    QString strLayer;
    int index;
    variantDxfEntity entity;
    stuSelectedEntity()
    {
        type = enumEntity_None;
        strLayer = "";
        index = -1;
    }
};



//图层类
struct stuLayer
{
    QColor color;
	vector<Point> vecPoints;
    vector<Line> vecLines;
    vector<Circle> vecCircles;
    vector<Arc> vecArcs;
    vector<Polyline> vecPolylines;
    vector<Text> vecTexts;
};

//图层类,图元不做区分
struct stuLayoutMix
{
	vector<variantDxfEntity> vecEntities;
};

class CDxfStruct
{
public:
	CDxfStruct();
	~CDxfStruct();

private:




public:

};



#endif // DxfStruct_h