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