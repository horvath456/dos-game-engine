#ifndef ENGINE_FONT_H
#define ENGINE_FONT_H

#pragma once

namespace Engine
{
    class Font
    {
    public:
        Font(const char *filename);
        ~Font();

        unsigned char far *get_buffer(char c);

        unsigned char width;
        unsigned char height;

    private:
        unsigned char far *buffer;
    };
};

#endif // ENGINE_FONT_H