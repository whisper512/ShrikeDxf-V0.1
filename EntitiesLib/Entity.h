#pragma once
#include "EntityBase.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "Arc.h"
#include "Ellipse.h"
#include "LWPolyline.h"
#include "Polyline.h"
#include "Spline.h"
#include "Text.h"
#include "MText.h"
#include "Insert.h"
#include "Soild.h"
#include "Hatch.h"


using variantDxfEntity = std::variant<
    EntityPoint,
    EntityLine,
    EntityCircle,
    EntityArc,
    EntityEllipse,
    EntityLWPolyline,
    EntityPolyline,
    EntitySpline,
    EntityText,
    EntityMText,
    EntityInsert,
    EntitySolid,
    EntityHatch
>;

inline EntityType GetEntityType(const variantDxfEntity& ent)
{
    return static_cast<EntityType>(ent.index());
}

inline EntityProp& GetProp(variantDxfEntity& ent)
{
    return std::visit([](auto& e) -> EntityProp& { return e.prop; }, ent);
}
inline const EntityProp& GetProp(const variantDxfEntity& ent)
{
    return std::visit([](const auto& e) -> const EntityProp& { return e.prop; }, ent);
}