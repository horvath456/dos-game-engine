#ifndef ANIMOBJ_H
#define ANIMOBJ_H

#pragma once

#include "screen.h"

class AnimationObject
{
public:
    AnimationObject(const char *filename);
    ~AnimationObject();

    void draw(Screen &screen, int x, int y);

    unsigned char far *get_buffer();

    unsigned int frame;

private:
    unsigned int num_frames;

    unsigned int width;
    unsigned int height;

    unsigned char far *buffer;
};

#endif