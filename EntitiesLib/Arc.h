#pragma once
#include "EntityBase.h"

struct CanonicalArc {
    double s;      // 规范化后的起始角
    double e;      // 规范化后的终止角
    double sweep;  // 扫掠角（0 ~ 2π）
};

// ─── Arc ────────────────────────────────────────────────────
struct EntityArc
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;
    double     startAngle = 0.0;  // 弧度
    double     endAngle = 0.0;    // 弧度
    bool       isCCW = true;      // 注意在实体弧中 isCCW 恒为 1（仅 hatch 边界用）

public:
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到弧的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);

private:
    // 将弧统一化为"逆时针从startAngle到endAngle的规范形式（s < e 且 sweep < 2π）
    CanonicalArc canonicalArc() const;  // 获取规范化的弧

};

// 三点确定圆弧
bool ThreePointsToArc(QPointF p1, QPointF p2, QPointF p3, QPointF& center, qreal& radius, qreal& startAngle, qreal& endAngle);
