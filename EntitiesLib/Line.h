#pragma once
#include "EntityBase.h"

// ─── Line ───────────────────────────────────────────────────
struct EntityLine
{
    EntityProp prop;
    Vertex3D   startPoint, endPoint;

    double length() const {
        double dx = endPoint.x() - startPoint.x();
        double dy = endPoint.y() - startPoint.y();
        return std::sqrt(dx * dx + dy * dy);
    }
    double angle() const {
        return std::atan2(endPoint.y() - startPoint.y(),
            endPoint.x() - startPoint.x());
    }
};