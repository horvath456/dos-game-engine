#ifndef ENGINE_SPRSHEET_H
#define ENGINE_SPRSHEET_H

#pragma once

namespace Engine
{
    class Spritesheet
    {
    public:
        Spritesheet(const char *filename);
        ~Spritesheet();

        unsigned char far *get_buffer_at(unsigned int x, unsigned int y);
        unsigned char pixel_at(unsigned int x, unsigned int y);

        unsigned int width;
        unsigned int height;

    private:
        unsigned char far *buffer;
    };
};

#endif // ENGINE_SPRSHEET_H