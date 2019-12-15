#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include "screen.h"
#include "drawobj.h"
#include <vector>

class Player;

class Level
{
public:
    Level(const char *filename);
    ~Level();

    void add_tile(DrawObject *d);

    void render(Screen &screen, Player& player);

    unsigned char get_tile(double x, double y);

    int camera_x;
    int camera_y;

    unsigned int level_width;
    unsigned int level_height;

private:
    unsigned char far *buffer;
    std::vector<void *> tilemap;
};

#endif