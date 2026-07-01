#include "Soild.h"

QPointF EntitySolid::centerPoint() const
{
    QRectF bb = boundingBox();
    return bb.center();
}

QRectF EntitySolid::boundingBox(double padding ) const
{
    double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
    for (int i = 0; i < 4; ++i)
    {
        minX = std::min(minX, corner[i].x());
        minY = std::min(minY, corner[i].y());
        maxX = std::max(maxX, corner[i].x());
        maxY = std::max(maxY, corner[i].y());
    }
    return QRectF(minX - padding, minY - padding, maxX - minX + padding * 2, maxY - minY + padding * 2);
}


double EntitySolid::distanceTo(double px, double py) const {
    // 用包围盒近似
    double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
    for (int i = 0; i < 4; ++i) {
        minX = std::min(minX, corner[i].x());
        minY = std::min(minY, corner[i].y());
        maxX = std::max(maxX, corner[i].x());
        maxY = std::max(maxY, corner[i].y());
    }
    if (px >= minX && px <= maxX && py >= minY && py <= maxY) return 0.0;
    double dLeft = std::abs(px - minX), dRight = std::abs(px - maxX);
    double dTop = std::abs(py - minY), dBottom = std::abs(py - maxY);
    if (px >= minX && px <= maxX) return std::min(dTop, dBottom);
    if (py >= minY && py <= maxY) return std::min(dLeft, dRight);
    return std::sqrt(std::min({ dLeft * dLeft + dTop * dTop, dLeft * dLeft + dBottom * dBottom,
                               dRight * dRight + dTop * dTop, dRight * dRight + dBottom * dBottom }));
}

void EntitySolid::translate(double dx, double dy)
{
    for (int i = 0; i < 4; ++i) {
        corner[i].setX(corner[i].x() + dx);
        corner[i].setY(corner[i].y() + dy);
    }
}


void EntitySolid::mirrorX()
{
    for (int i = 0; i < 4; ++i) {
        corner[i].setY(-corner[i].y());
    }
}

void EntitySolid::mirrorY()
{
    for (int i = 0; i < 4; ++i) {
        corner[i].setX(-corner[i].x());
    }
}

void EntitySolid::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    for (int i = 0; i < 4; ++i) {
        double dx = corner[i].x() - cx;
        double dy = corner[i].y() - cy;
        corner[i].setX(cx + dx * cosA - dy * sinA);
        corner[i].setY(cy + dx * sinA + dy * cosA);
    }
}
