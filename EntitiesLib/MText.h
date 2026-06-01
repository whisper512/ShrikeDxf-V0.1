#pragma once
#include "EntityBase.h"
#include <QRegularExpression>
#include <QGraphicsItem>

// ─── MText ──────────────────────────────────────────────────
struct EntityMText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   xAxisDir;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;
    double     widthFactor = 1.0;
    double     lineSpacing = 1.0;
    std::string style = "STANDARD";
    int     attachPoint = 1;
    int     textDir = 1;
    int     lineSpaceStyle = 1;

    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
};