#ifndef DRAWOBJ_H
#define DRAWOBJ_H

#pragma once

#include "screen.h"

class DrawObject
{
public:
    DrawObject(const char *filename);
    ~DrawObject();

    void draw(Screen &screen, int x, int y, bool transparent = false);

    unsigned char far *get_buffer();

    unsigned int width;
    unsigned int height;

    unsigned int x;
    unsigned int y;

private:
    unsigned char pixel_at(unsigned int x, unsigned int y);

    unsigned char far *buffer;
};

#endif