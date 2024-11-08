#ifndef ANIMOBJ_H
#define ANIMOBJ_H

#pragma once

#include "screen.h"
#include "sprite.h"

class AnimationObject
{
public:
    AnimationObject(Sprite *sprites[], unsigned int num_sprites, int x = 0, int y = 0);

    void draw(Screen &screen);

    unsigned char far *get_buffer();

    void set_position(int x, int y);
    void set_frame(unsigned int f);

    unsigned int frame;

    unsigned int width;
    unsigned int height;

    int x;
    int y;

private:
    unsigned int num_frames;
    Sprite *frames[32];
};

#endif