#ifndef MOVABLE_H
#define MOVABLE_H

#pragma once

#include "vector2d.h"
#include "aabb.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

class Movable
{
public:
    void update_physics(double delta_time)
    {
        old_pos = pos;
        old_speed = speed;

        was_on_ground = on_ground;
        pushed_right_wall = pushes_right_wall;
        pushed_left_wall = pushes_left_wall;
        was_at_ceiling = at_ceiling;

        pos.x += speed.x * delta_time;
        pos.y += speed.y * delta_time;

        if (pos.y < 0.0f)
        {
            pos.y = 0.0f;
            on_ground = TRUE;
        }
        else
            on_ground = FALSE;

        aabb.center.x = pos.x + aabb_offset.x;
        aabb.center.y = pos.y + aabb_offset.y;
    }

    int get_pos_x() { return (int)fabs(pos.x); }
    int get_pos_y() { return (int)fabs(pos.y); }

    Vector2D old_pos;
    Vector2D pos;
    Vector2D old_speed;
    Vector2D speed;

    AABB aabb;
    Vector2D aabb_offset;

    int pushed_right_wall;
    int pushes_right_wall;

    int pushed_left_wall;
    int pushes_left_wall;

    int was_on_ground;
    int on_ground;

    int was_at_ceiling;
    int at_ceiling;
};

#endif