#ifndef LEVEL_H
#define LEVEL_H

#pragma once

#include "screen.h"
#include "drawobj.h"
#include <vector>

class Level
{
public:
    Level(const char *filename);
    ~Level();

    void add_tile(DrawObject *d);

    void draw(Screen &screen);

private:
    unsigned int width;
    unsigned int height;

    int x;
    int y;

    unsigned char far *buffer;
    std::vector<void *> tilemap;
};

#endif