#ifndef DRAW_H
#define DRAW_H

#pragma once

#include "screen.h"
#include "sprite.h"

namespace Draw
{
    void putpixel(Screen &screen, unsigned int x, unsigned int y, unsigned char color);
    void hline(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned char color);
    void vline(Screen &screen, unsigned int x, unsigned int y, unsigned int height, unsigned char color);
    void rect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
    void fillrect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color);
    void rect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color, unsigned char radius = 2);
    void fillrect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width,
                        unsigned int height, unsigned char color, unsigned char outline_color, unsigned char radius = 2);
    void dithering_pattern(Screen &screen, unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height, unsigned char color, unsigned char density); // ignore density for now, assume height>=2, width >=2
    void draw_sprite_buffer(Screen &screen, unsigned char far *buffer, int x, int y, unsigned int width, unsigned int height, bool transparent = false);
    void draw_sprite(Screen &screen, Sprite &sprite, int x, int y, bool transparent = false);
};

#endif