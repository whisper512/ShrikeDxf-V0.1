#include "Insert.h"

QPointF EntityInsert::centerPoint() const
{
    return QPointF(insertPoint.x(), insertPoint.y());
}


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

void EntityInsert::stretch(StretchGrip grip, const QPointF& newPos)
{
    QRectF bb = boundingBox();
    QPointF anchor, oldCorner;

    switch (grip) {
    case StretchGrip::TopRight:
        anchor = bb.bottomLeft();
        oldCorner = bb.topRight();
        break;
    case StretchGrip::TopLeft:
        anchor = bb.bottomRight();
        oldCorner = bb.topLeft();
        break;
    case StretchGrip::BottomRight:
        anchor = bb.topLeft();
        oldCorner = bb.bottomRight();
        break;
    case StretchGrip::BottomLeft:
        anchor = bb.topRight();
        oldCorner = bb.bottomLeft();
        break;
    default: return;
    }

    double oldDiag = QLineF(anchor, oldCorner).length();
    double newDiag = QLineF(anchor, newPos).length();
    if (oldDiag < 1e-9) return;

    double s = newDiag / oldDiag;

    // 插入点相对锚点缩放
    insertPoint.setX(anchor.x() + (insertPoint.x() - anchor.x()) * s);
    insertPoint.setY(anchor.y() + (insertPoint.y() - anchor.y()) * s);

    // 块均匀缩放
    xScale *= s;
    yScale *= s;
    // rotation 不变，col/row 不变
}

