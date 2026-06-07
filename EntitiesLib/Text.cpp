#include "Text.h"

// 计算边界(场景坐标Y-down)
QRectF EntityText::boundingBox(double padding ) const
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


double EntityText::distanceTo(double px, double py) const
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

void EntityText::translate(double dx, double dy) 
{
    insertPoint.setX(insertPoint.x() + dx);
    insertPoint.setY(insertPoint.y() + dy);
    if (!alignPoint.isNull()) {   // 如果定义了对齐点
        alignPoint.setX(alignPoint.x() + dx);
        alignPoint.setY(alignPoint.y() + dy);
    }
}