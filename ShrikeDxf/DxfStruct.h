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

    //根据qcolor获取dxf颜色索引
    static int getColorIndex(const QColor& color)
    {
        const auto& colorMap = getColorMap();

        // 首先检查是否完全匹配
        for (const auto& pair : colorMap) {
            if (pair.second == color) {
                return pair.first;
            }
        }

        // 如果没有完全匹配，则寻找最接近的颜色
        int minDistance = INT_MAX;
        int closestIndex = 7; // 默认返回白色

        for (const auto& pair : colorMap) {
            int rDiff = abs(pair.second.red() - color.red());
            int gDiff = abs(pair.second.green() - color.green());
            int bDiff = abs(pair.second.blue() - color.blue());
            int distance = rDiff + gDiff + bDiff;

            if (distance < minDistance) {
                minDistance = distance;
                closestIndex = pair.first;
            }
        }

        return closestIndex;
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

struct stuPreviewEntity
{
    enumPreviewEntity type;
    QString strLayer;
    stuPreviewEntity()
    {
        type = enumPreviewEntity_None;
        strLayer = "";
    }
};

static enumEntity GetVariantDxfEntity(variantDxfEntity dxfEntity, Point& point, Line& line, Circle& circle, Arc& arc, Polyline& polyline)
{
    enumEntity EntityType = enumEntity_None;
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Point>) {
            point = arg;
            EntityType = enumEntity_Point;
        }
        else if constexpr (std::is_same_v<T, Line>) {
            line = arg;
            EntityType = enumEntity_Line;
        }
        else if constexpr (std::is_same_v<T, Circle>) {
            circle = arg;
            EntityType = enumEntity_Circle;
        }
        else if constexpr (std::is_same_v<T, Arc>) {
            arc = arg;
            EntityType = enumEntity_Arc;
        }
        else if constexpr (std::is_same_v<T, Polyline>) {
            polyline = arg;
            EntityType = enumEntity_Polyline;
        }
        }, dxfEntity);
    return EntityType;
}



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