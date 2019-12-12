#ifndef AABB_H
#define AABB_H

#include <math.h>
#include "vector2d.h"

class AABB
{
public:
    AABB(Vector2D _center, Vector2D _half_size)
    {
        center = _center;
        half_size = _half_size;
    }

    AABB()
    {
        center = Vector2D();
        half_size = Vector2D();
    }

    bool overlaps(AABB &other)
    {
        if (fabs(center.x - other.center.x) > half_size.x + other.half_size.x)
            return false;
        if (fabs(center.y - other.center.y) > half_size.y + other.half_size.y)
            return false;
        return true;
    }

    Vector2D center;
    Vector2D half_size;
};

#endif