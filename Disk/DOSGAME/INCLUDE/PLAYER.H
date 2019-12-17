#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "animobj.h"

class Level;

class Player
{
public:
    Player(const char *filename);
    ~Player();

    void update(double delta_time, Level &level);

    double pos_x;
    double pos_y;

    AnimationObject *anim_obj;

private:
    void handle_collision(unsigned int tile_x, unsigned int tile_y, bool handle_y);
    double new_pos_x;
    double new_pos_y;
    double vel_x;
    double vel_y;
    int dir_mod_x;
    int dir_mod_y;
    bool on_ground;
};
#endif