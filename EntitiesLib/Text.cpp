#include "Text.h"

QPointF EntityText::centerPoint() const
{
    QRectF bb = boundingBox();
    return bb.center();
}


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


void EntityText::mirrorX()
{
    insertPoint.setY(-insertPoint.y());
    if (!alignPoint.isNull())
        alignPoint.setY(-alignPoint.y());

    rotation = -rotation;       // 文字倾角取反
    obliqueAngle = -obliqueAngle;   // 斜体方向反转
}

void EntityText::mirrorY()
{
    insertPoint.setX(-insertPoint.x());
    if (!alignPoint.isNull())
        alignPoint.setX(-alignPoint.x());

    rotation = M_PI - rotation; // 文字倾角关于 Y 镜像
    obliqueAngle = -obliqueAngle;   // 斜体方向反转
}

void EntityText::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    // 旋转插入点
    double dx = insertPoint.x() - cx;
    double dy = insertPoint.y() - cy;
    insertPoint.setX(cx + dx * cosA - dy * sinA);
    insertPoint.setY(cy + dx * sinA + dy * cosA);

    // 旋转对齐点（如果存在）
    if (!alignPoint.isNull()) {
        dx = alignPoint.x() - cx;
        dy = alignPoint.y() - cy;
        alignPoint.setX(cx + dx * cosA - dy * sinA);
        alignPoint.setY(cy + dx * sinA + dy * cosA);
    }

    rotation += angle;
    // obliqueAngle 不变：纯旋转不改变文字自身斜体方向
}
