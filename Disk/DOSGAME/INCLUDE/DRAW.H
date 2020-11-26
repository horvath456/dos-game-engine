#ifndef DRAW_H
#define DRAW_H

#pragma once

#include "screen.h"

namespace Draw
{
    void putpixel(Screen &screen, unsigned int x, unsigned int y, unsigned char color);
    void hline(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned char color);
    void vline(Screen &screen, unsigned int x, unsigned int y, unsigned int height, unsigned char color);
    void rect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
    void fillrect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
    void rect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
    void fillrect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color, unsigned char outline_color);
}; // namespace Draw

#endif