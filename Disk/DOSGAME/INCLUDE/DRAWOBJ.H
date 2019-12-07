#ifndef DRAWOBJ_H
#define DRAWOBJ_H

#pragma once

#include <fstream.h>
#include <iostream.h>

#include "screen.h"

#define DEFAULT_WIDTH 16
#define DEFAULT_HEIGHT 16

class DrawObject
{
public:
    DrawObject(unsigned int width = DEFAULT_WIDTH, unsigned int height = DEFAULT_HEIGHT);
    DrawObject(const char *filename);
    ~DrawObject();

    void draw(Screen &screen, int x, int y);

    unsigned char far *get_buffer();

private:
    unsigned char pixel_at(unsigned int x, unsigned int y);

    unsigned int width;
    unsigned int height;
    unsigned char far *buffer;
};

#endif