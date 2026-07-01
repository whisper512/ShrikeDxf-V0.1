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

void EntityLine::stretch(StretchGrip grip, const QPointF& newPos)
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

    startPoint.setX(anchor.x() + (startPoint.x() - anchor.x()) * s);
    startPoint.setY(anchor.y() + (startPoint.y() - anchor.y()) * s);
    endPoint.setX(anchor.x() + (endPoint.x() - anchor.x()) * s);
    endPoint.setY(anchor.y() + (endPoint.y() - anchor.y()) * s);
}
