#include "Circle.h"

QRectF EntityCircle::boundingBox(double padding) const
{
    std::vector<QPointF> pts = {
        QPointF(center.x() - radius, center.y() - radius),
        QPointF(center.x() + radius, center.y() + radius)
    };
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityCircle::distanceTo(double px, double py) const
{
    double dx = px - center.x(), dy = py - center.y();
    double d = std::sqrt(dx * dx + dy * dy);
    return std::abs(d - radius);
}

void EntityCircle::translate(double dx, double dy) 
{
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
}

void EntityCircle::mirrorX()
{
    center.setY(-center.y());

}

void EntityCircle::mirrorY()
{
    center.setX(-center.x());
}

void EntityCircle::stretch(StretchGrip grip, const QPointF& newPos)
{
    if (grip == StretchGrip::None) return;

    QRectF bb = boundingBox();    // 正方形包围盒
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

    double scale = newDiag / oldDiag;

    center.setX(anchor.x() + (center.x() - anchor.x()) * scale);
    center.setY(anchor.y() + (center.y() - anchor.y()) * scale);
    radius *= scale;
}
