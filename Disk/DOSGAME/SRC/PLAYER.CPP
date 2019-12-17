#include "player.h"

#include "screen.h"
#include "keyboard.h"
#include "drawobj.h"
#include "level.h"
#include <iostream.h>
#include <cmath>

#define TILE_WIDTH 16.0
#define TILE_HEIGHT 16.0

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

void Player::handle_collision(unsigned int tile_x, unsigned int tile_y, bool handle_y)
{
    if (handle_y)
    {
        new_pos_y = pos_y;
        //if (tile_y * TILE_HEIGHT >= pos_y + anim_obj->height)
            on_ground = true;
    }
    else
    {
        new_pos_x = pos_x;
    }
}

void Player::update(double delta_time, Level &level)
{
    if (Keyboard::check_if_pressed(KEY_LEFT))
    {
        vel_x += (on_ground ? -25.0 : -15.0) * delta_time;
    }
    if (Keyboard::check_if_pressed(KEY_RIGHT))
    {
        vel_x += (on_ground ? 25.0 : 15.0) * delta_time;
    }
    if (Keyboard::get_key_down(KEY_SPACE) && on_ground)
    {
        vel_y = -60.0 * delta_time;
    }

    // Gravity
    if (!on_ground)
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
    new_pos_x = pos_x + vel_x * delta_time;
    new_pos_y = pos_y + vel_y * delta_time;

    on_ground = false;

    int left_tile = (int)(new_pos_x / (double)TILE_WIDTH);
    int right_tile = (int)((double)(new_pos_x + anim_obj->width) / (double)TILE_WIDTH);
    int top_tile = (int)(pos_y / (double)TILE_HEIGHT);
    int bottom_tile = (int)((double)(pos_y + anim_obj->height) / (double)TILE_HEIGHT);

    if (left_tile < 0)
        left_tile = 0;
    if (right_tile > level.level_width)
        right_tile = level.level_width;
    if (top_tile < 0)
        top_tile = 0;
    if (bottom_tile > level.level_height)
        bottom_tile = level.level_height;

    for (unsigned int i = left_tile; i <= right_tile; i++)
        for (unsigned int j = top_tile; j <= bottom_tile; j++)
            if (level.get_tile(i, j) != 0)
                handle_collision(i, j, false);

    top_tile = (int)((double)new_pos_y / (double)TILE_HEIGHT);
    bottom_tile = (int)((double)(new_pos_y + anim_obj->height) / (double)TILE_HEIGHT);

    if (top_tile < 0)
        top_tile = 0;
    if (bottom_tile > level.level_height)
        bottom_tile = level.level_height;

    for (unsigned int i = left_tile; i <= right_tile; i++)
        for (unsigned int j = top_tile; j <= bottom_tile; j++)
            if (level.get_tile(i, j) != 0)
                handle_collision(i, j, true);

    // Apply new position
    pos_x = new_pos_x;
    pos_y = new_pos_y;
}