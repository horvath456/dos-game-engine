#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include "screen.h"
#include "drawobj.h"
#include "player.h"
#include <vector>

class Level
{
public:
    Level(const char *filename);
    ~Level();

    void add_tile(DrawObject *d);

    void set_player(Player* pl);

    void render(Screen &screen);

    int camera_x;
    int camera_y;

    unsigned int level_width;
    unsigned int level_height;

private:
    unsigned char get_tile(unsigned int x, unsigned int y);

    unsigned char far *buffer;
    std::vector<void *> tilemap;
    Player* player;
};

#endif