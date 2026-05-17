#pragma once
#include "EntityBase.h"
#include <QFontMetricsF>
#include <QGraphicsTextItem>

// ─── Text ───────────────────────────────────────────────────
struct EntityText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   alignPoint;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;   // 弧度
    double     widthFactor = 1.0;
    double     obliqueAngle = 0.0;
    std::string style = "STANDARD";
    int     textGen = 0;
    int     alignH = 0;
    int     alignV = 0;

    // 计算边界(场景坐标Y-down)
    QRectF boundingBox(double padding = 0.0) const
    {
        QFont font(QString::fromStdString(style));
        if (height < 1.0) font.setPixelSize(1);
        else font.setPixelSize(static_cast<int>(height));
        
        QGraphicsTextItem tempItem;
        tempItem.setPlainText(QString::fromStdString(text));
        tempItem.setFont(font);
        QRectF lr = tempItem.boundingRect();
        QFontMetricsF fm(font);
        double ascent = fm.ascent();
        double top = insertPoint.y() + lr.top() + ascent - lr.bottom();
        double left = insertPoint.x();
        double right = insertPoint.x() - lr.left() + lr.right();
        double bottom = insertPoint.y() + ascent;
        return QRectF(left - padding, top - padding,
            right - left + padding * 2, bottom - top + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {
        QFont font(QString::fromStdString(style));
        if (height < 1.0) font.setPixelSize(1);
        else font.setPixelSize(static_cast<int>(height));
        QGraphicsTextItem tempItem;
        tempItem.setPlainText(QString::fromStdString(text));
        tempItem.setFont(font);
        QRectF lr = tempItem.boundingRect();
        QFontMetricsF fm(font);
        double ascent = fm.ascent();
        // 和 boundingBox 完全一致的矩形
        double top = insertPoint.y() + lr.top() + ascent - lr.bottom();
        double left = insertPoint.x();
        double right = insertPoint.x() - lr.left() + lr.right();
        double bottom = insertPoint.y() + ascent;
        if (px >= left && px <= right && py >= top && py <= bottom)
            return 0.0;
        double dLeft = std::abs(px - left);
        double dRight = std::abs(px - right);
        double dTop = std::abs(py - top);
        double dBottom = std::abs(py - bottom);
        if (px >= left && px <= right) return std::min(dTop, dBottom);
        if (py >= top && py <= bottom) return std::min(dLeft, dRight);
        return EntUtil::rectDistanceToPoint(px, py, left, top, right, bottom);
    }

};