#ifndef ANIMOBJ_H
#define ANIMOBJ_H

#pragma once

#include <fstream.h>
#include <iostream.h>

#include "screen.h"
#include "drawobj.h"

class AnimationObject
{
public:
    AnimationObject(unsigned int numframes, const char **filenames);
    ~AnimationObject();

    // x >= 0 && y >= 0 && x+width<SCREEN_WIDTH && y+height<SCREEN_HEIGHT
    void draw(Screen &screen, int x, int y, int transparent = 1);

    unsigned int frame;

private:
    unsigned int width;
    unsigned int height;
    
    unsigned int num_frames;

    DrawObject **draw_objects;
}

#endif