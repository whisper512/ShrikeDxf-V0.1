#include <cmath>

#include "Ellipse.h"

QRectF EntityEllipse::boundingBox(double padding) const
{
    double majorLen = std::sqrt(majorAxisEndpoint.x() * majorAxisEndpoint.x() + majorAxisEndpoint.y() * majorAxisEndpoint.y());
    if (majorLen < 1e-10) return QRectF();
    double minorLen = majorLen * ratio;
    double angleRad = std::atan2(majorAxisEndpoint.y(), majorAxisEndpoint.x());
    double cosA = std::cos(angleRad), sinA = std::sin(angleRad);
    double cx = center.x(), cy = center.y();
    std::vector<QPointF> pts;
    for (int i = 0; i <= 64; ++i)
    {
        double theta = startParam + (endParam - startParam) * i / 64.0;
        double lx = majorLen * std::cos(theta);
        double ly = minorLen * std::sin(theta);
        pts.emplace_back(cx + lx * cosA - ly * sinA,
            cy + lx * sinA + ly * cosA);
    }
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityEllipse::distanceTo(double px, double py) const 
{

    double majorLen = std::sqrt(
        majorAxisEndpoint.x() * majorAxisEndpoint.x() +
        majorAxisEndpoint.y() * majorAxisEndpoint.y());
    if (majorLen < 1e-10) return 1e10;
    double minorLen = majorLen * ratio;
    double angleRad = std::atan2(majorAxisEndpoint.y(), majorAxisEndpoint.x());
    double cosA = std::cos(-angleRad), sinA = std::sin(-angleRad);
    double lx = (px - center.x()) * cosA - (py - center.y()) * sinA;
    double ly = (px - center.x()) * sinA + (py - center.y()) * cosA;
    double val = (lx * lx) / (majorLen * majorLen) + (ly * ly) / (minorLen * minorLen);
    if (val < 1e-6) return 0.0;
    return std::abs(std::sqrt(val) - 1.0) * std::min(majorLen, minorLen);
}

void EntityEllipse::translate(double dx, double dy) 
{
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
}


static void normalizeAngles(double& start, double& end)
{
    const double twoPi = 2.0 * M_PI;
    // 将 start, end 映射到 [0, twoPi) 区间
    start = std::fmod(start, twoPi);
    if (start < 0) start += twoPi;
    end = std::fmod(end, twoPi);
    if (end < 0) end += twoPi;
    // 保证 end > start (逆时针)
    if (end <= start)
        end += twoPi;
}


void EntityEllipse::mirrorX()
{
    center.setY(-center.y());
    majorAxisEndpoint.setY(-majorAxisEndpoint.y());

    // 参数变换：t' = -t
    double newStart = -endParam;
    double newEnd = -startParam;
    startParam = newStart;
    endParam = newEnd;
    normalizeAngles(startParam, endParam);
}


void EntityEllipse::mirrorY()
{
    center.setX(-center.x());
    majorAxisEndpoint.setX(-majorAxisEndpoint.x());

    // 参数变换：t' = π - t
    double newStart = M_PI - endParam;
    double newEnd = M_PI - startParam;
    startParam = newStart;
    endParam = newEnd;
    normalizeAngles(startParam, endParam);
}


void EntityEllipse::rotate(double angle, const QPointF& rotCenter)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);

    // 旋转中心点
    double dx = center.x() - rotCenter.x();
    double dy = center.y() - rotCenter.y();
    center.setX(rotCenter.x() + dx * cosA - dy * sinA);
    center.setY(rotCenter.y() + dx * sinA + dy * cosA);

    // 旋转主轴向量(相对中心的偏移)
    double mx = majorAxisEndpoint.x();
    double my = majorAxisEndpoint.y();
    majorAxisEndpoint.setX(mx * cosA - my * sinA);
    majorAxisEndpoint.setY(mx * sinA + my * cosA);

    // 参数角同步旋转(跟随刚体旋转)
    startParam += angle;
    endParam += angle;
    normalizeAngles(startParam, endParam);
}

void EntityEllipse::stretch(StretchGrip grip, const QPointF& newPos)
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

    double scale = newDiag / oldDiag;

    // 中心等比偏移
    center.setX(anchor.x() + (center.x() - anchor.x()) * scale);
    center.setY(anchor.y() + (center.y() - anchor.y()) * scale);

    // 主轴向量等比缩放（向量，直接乘）
    majorAxisEndpoint.setX(majorAxisEndpoint.x() * scale);
    majorAxisEndpoint.setY(majorAxisEndpoint.y() * scale);

    // ratio 不变——均匀缩放保持形状
}
