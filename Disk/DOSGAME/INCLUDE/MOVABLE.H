#ifndef MOVABLE_H
#define MOVABLE_H

#pragma once

#include "vector2d.h"
#include "aabb.h"
#include <math.h>

#define TRUE 1
#define FALSE 0

#define GRAVITY 2
#define MAX_FALLING_SPEED 10
#define JUMP_SPEED 10
#define SPEED 4

class Movable
{
public:
    Movable()
    {
    }

    void update_physics(double delta_time)
    {
        old_pos = pos;
        old_velocity = velocity;

        was_on_ground = on_ground;
        pushed_right_wall = pushes_right_wall;
        pushed_left_wall = pushes_left_wall;
        was_at_ceiling = at_ceiling;

        if (target_velocity.y < 0 && on_ground)
        {
            velocity.y = target_velocity.y * JUMP_SPEED;
        }

        if (target_velocity.y < 0 || !on_ground)
        {
            velocity.y = (velocity.y + GRAVITY * delta_time) > MAX_FALLING_SPEED ? MAX_FALLING_SPEED : (velocity.y + GRAVITY * delta_time);
        }
        else
        {
            velocity.y = 0;
        }

        if (target_velocity.x > 0)
        {
            if (pushes_right_wall)
                velocity.x = 0.0;
            else
                velocity.x = target_velocity.x * SPEED;
        }

        if (target_velocity.x < 0)
        {
            if (pushes_left_wall)
                velocity.x = 0.0;
            else
                velocity.x = target_velocity.x * SPEED;
        }

        if (target_velocity.x == 0)
        {
            velocity.x = 0;
        }

        movement(Vector2D(velocity.x * delta_time, (double)0));
        movement(Vector2D((double)0, velocity.y * delta_time));

        on_ground = pos.y >= 145;

        aabb.center = pos + aabb_offset;

        target_velocity = Vector2D();
    }

    int get_pos_x() { return (int)ceil(pos.x); }
    int get_pos_y() { return (int)ceil(pos.y); }

    Vector2D old_pos;
    Vector2D pos;
    Vector2D old_velocity;
    Vector2D velocity;

    Vector2D target_velocity;

    AABB aabb;
    Vector2D aabb_offset;

    int pushed_right_wall;
    int pushes_right_wall;

    int pushed_left_wall;
    int pushes_left_wall;

    int was_on_ground;
    int on_ground;

    int grounded;

    int was_at_ceiling;
    int at_ceiling;

private:
    void movement(Vector2D move)
    {
        pos = pos + move;
    }
};

#endif