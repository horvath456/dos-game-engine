#ifndef ENGINE_SPRITE_H
#define ENGINE_SPRITE_H

#pragma once

#include "sprsheet.h"

namespace Engine
{
    class Sprite
    {
    public:
        Sprite(Spritesheet &spritesheet, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
        Sprite(const char *filename);
        ~Sprite();

        unsigned char far *get_buffer();
        unsigned char pixel_at(unsigned int x, unsigned int y);

        unsigned int width;
        unsigned int height;

    private:
        unsigned char far *buffer;
    };
};

#endif // ENGINE_SPRITE_H