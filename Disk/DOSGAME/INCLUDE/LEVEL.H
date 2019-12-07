#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include "player.h"
#include "drawobj.h"
#include "screen.h"

class Level
{
public:
    Level(const char *filename, Player &pl);
    void draw(Screen &screen);

    long int x;
    long int y;

private:
    Player &player;
    DrawObject far *draw_objects;

    unsigned char far *level_data;
    unsigned int cnt_level_elements;
};

#endif