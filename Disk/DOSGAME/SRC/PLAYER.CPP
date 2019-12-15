#include "player.h"

#include "screen.h"
#include "keyboard.h"
#include "drawobj.h"
#include "level.h"
#include <iostream.h>
#include <cmath>

Player::Player(const char *filename)
{
    anim_obj = new AnimationObject(filename);
    pos_x = 10;
    pos_y = 10;
    vel_x = 0;
    vel_y = 0;
}

Player::~Player()
{
    delete anim_obj;
}

void Player::update(double delta_time, Level &level)
{
    if (Keyboard::check_if_pressed(KEY_UP))
    {
        vel_y = -6.0;
    }
    if (Keyboard::check_if_pressed(KEY_DOWN))
    {
        vel_y = 6.0;
    }
    if (Keyboard::check_if_pressed(KEY_LEFT))
    {
        vel_x += (on_ground ? -25.0 : -15.0) * delta_time;
        dir_mod_y = 1;
    }
    if (Keyboard::check_if_pressed(KEY_RIGHT))
    {
        vel_x += (on_ground ? 25.0 : 15.0) * delta_time;
        dir_mod_y = 0;
    }
    if (Keyboard::get_key_down(KEY_SPACE))
    {
        if (vel_y == 0)
        {
            vel_y = -12.0;
            dir_mod_x = 1;
        }
    }

    // Gravity
    vel_y += 20.0 * delta_time;

    // Drag
    if (on_ground)
    {
        vel_x += -3.0 * vel_x * delta_time;
        if (std::fabs(vel_x) < 0.01)
            vel_x = 0.0;
    }

    // Clamp velocities
    if (vel_x > 10.0)
        vel_x = 10.0;

    if (vel_x < -10.0)
        vel_x = -10.0;

    if (vel_y > 100.0)
        vel_y = 100.0;

    if (vel_y < -100.0)
        vel_y = -100.0;

    // Calculate potential new position
    double new_pos_x = pos_x + vel_x * delta_time;
    double new_pos_y = pos_y + vel_y * delta_time; /*

    if (vel_x <= 0) // Moving Left
    {
        if (level.get_tile(new_pos_x + 0.0, pos_y + 0.0) != 0 || level.get_tile(new_pos_x + 0.0, pos_y + 0.99) != 0)
        {
            new_pos_x = (int)new_pos_x + 1;
            vel_x = 0;
        }
    }
    else // Moving Right
    {
        if (level.get_tile(new_pos_x + 1.0, pos_y + 0.0) != 0 || level.get_tile(new_pos_x + 1.0, pos_y + 0.99) != 0)
        {
            new_pos_x = (int)new_pos_x;
            vel_x = 0;
        }
    }*/

    on_ground = false;
    if (vel_y <= 0) // Moving Up
    {
        if (level.get_tile(new_pos_x + 0.0, new_pos_y) != 0 || level.get_tile(new_pos_x + 0.99, new_pos_y) != 0)
        {
            new_pos_y = (int)new_pos_y + 1;
            vel_y = 0;
        }
    }
    else // Moving Down
    {
        if (level.get_tile(new_pos_x, new_pos_y + anim_obj->height + 15.99) != 0 || level.get_tile(new_pos_x + anim_obj->width, new_pos_y + anim_obj->height + 15.99) != 0)
        {
            new_pos_y = (new_pos_y / 16) * 16 + -1;
            vel_y = 0;
            on_ground = true; // Player has a solid surface underfoot
            dir_mod_x = 0;
        }
    }

    // Apply new position
    pos_x = new_pos_x;
    pos_y = new_pos_y;
}