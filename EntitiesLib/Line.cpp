#include "Line.h"

double EntityLine::length() const
{
    double dx = endPoint.x() - startPoint.x();
    double dy = endPoint.y() - startPoint.y();
    return std::sqrt(dx * dx + dy * dy);
}

double EntityLine::angle() const
{
    return std::atan2(endPoint.y() - startPoint.y(),
        endPoint.x() - startPoint.x());
}

double EntityLine::distanceTo(double px, double py) const {
    return EntUtil::pointToSegmentDist(px, py,
        startPoint.x(), startPoint.y(),
        endPoint.x(), endPoint.y());
}

QRectF EntityLine::boundingBox(double padding) const {
    std::vector<QPointF> pts = {
        QPointF(startPoint.x(), startPoint.y()),
        QPointF(endPoint.x(),   endPoint.y())
    };
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

void EntityLine::translate(double dx, double dy) 
{
    startPoint.setX(startPoint.x() + dx);
    startPoint.setY(startPoint.y() + dy);
    endPoint.setX(endPoint.x() + dx);
    endPoint.setY(endPoint.y() + dy);
}

void EntityLine::mirrorX()
{
    startPoint.setY(-startPoint.y());
    endPoint.setY(-endPoint.y());
}

void EntityLine::mirrorY()
{
    startPoint.setX(-startPoint.x());
    endPoint.setX(-endPoint.x());
}

void EntityLine::rotate(double angle, const QPointF& center)
{
    double cosA = std::cos(angle);
    double sinA = std::sin(angle);

    auto rotatePoint = [&](Vertex3D& p) {
        double dx = p.x() - center.x();
        double dy = p.y() - center.y();
        double newX = center.x() + dx * cosA - dy * sinA;
        double newY = center.y() + dx * sinA + dy * cosA;
        p.setX(newX);
        p.setY(newY);
        };

    rotatePoint(startPoint);
    rotatePoint(endPoint);
}