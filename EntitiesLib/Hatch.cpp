#include "Hatch.h"

QRectF EntityHatch::boundingBox(double padding) const
{
    // 块引用没有固定大小,返回一个点
    return QRectF(0, 0, 0, 0);
}

double EntityHatch::distanceTo(double px, double py) const
{
    return 0.0;
}