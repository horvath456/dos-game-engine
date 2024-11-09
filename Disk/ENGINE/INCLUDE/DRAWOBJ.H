#ifndef ENGINE_DRAWOBJ_H
#define ENGINE_DRAWOBJ_H

#pragma once

#include "sprite.h"
#include "screen.h"

namespace Engine
{
    class DrawObject
    {
    public:
        DrawObject(Sprite &sprite, int x = 0, int y = 0, bool transparent = false);
        DrawObject(const char *filename, int x = 0, int y = 0, bool transparent = false);
        ~DrawObject();

        void set_position(int x, int y);
        void draw(Screen &screen);

        unsigned char far *get_buffer();

        unsigned int width;
        unsigned int height;

        int x;
        int y;

        bool transparent;

    private:
        unsigned char pixel_at(unsigned int x, unsigned int y);
        bool loaded_from_file;
        unsigned char far *buffer;
    };
};

#endif // ENGINE_DRAWOBJ_H