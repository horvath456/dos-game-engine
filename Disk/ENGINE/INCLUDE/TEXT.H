#ifndef ENGINE_TEXT_H
#define ENGINE_TEXT_H

#pragma once

#include "font.h"
#include "screen.h"

namespace Engine
{
    namespace Text
    {
        void write_text(const char *str, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y);
        void write_text_center(const char *str, Font &font, unsigned char color, Screen &screen, unsigned int y, unsigned int center_x = 160);
        void putchar(char c, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y);
    };
};

#endif // ENGINE_TEXT_H