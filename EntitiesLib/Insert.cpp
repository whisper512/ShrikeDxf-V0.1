#include "Insert.h"

QRectF EntityInsert::boundingBox(double padding) const
{
    std::vector<QPointF> pts = { QPointF(insertPoint.x(), insertPoint.y()) };
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityInsert::distanceTo(double px, double py) const 
{
    double dx = px - insertPoint.x(), dy = py - insertPoint.y();
    return std::sqrt(dx * dx + dy * dy);
}

void EntityInsert::translate(double dx, double dy) 
{
    insertPoint.setX(insertPoint.x() + dx);
    insertPoint.setY(insertPoint.y() + dy);
}


// ─── X 镜像（关于 X 轴翻转）────────────────────────────────
void EntityInsert::mirrorX()
{
    insertPoint.setY(-insertPoint.y());
    rotation = -rotation;
    yScale = -yScale;           // 块 Y 方向翻转
    rowSpacing = -rowSpacing;      // 阵列行方向翻转（行沿 Y 轴）
}

// ─── Y 镜像（关于 Y 轴翻转）────────────────────────────────
void EntityInsert::mirrorY()
{
    insertPoint.setX(-insertPoint.x());
    rotation = M_PI - rotation;
    xScale = -xScale;          // 块 X 方向翻转
    colSpacing = -colSpacing;      // 阵列列方向翻转（列沿 X 轴）
}

// ─── 旋转（绕指定中心点旋转）───────────────────────────────
void EntityInsert::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    double dx = insertPoint.x() - cx;
    double dy = insertPoint.y() - cy;
    insertPoint.setX(cx + dx * cosA - dy * sinA);
    insertPoint.setY(cy + dx * sinA + dy * cosA);

    rotation += angle;
    // 缩放和阵列间距不变
}
