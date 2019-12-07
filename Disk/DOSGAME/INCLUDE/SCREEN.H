#ifndef SCREEN_H
#define SCREEN_H

#pragma once

class Screen
{
public:
    Screen();
    ~Screen();

    void draw_screen_buffer();

    void clear_screen_buffer();

    void plot_pixel(unsigned int x, unsigned int y, unsigned char color);

private:
    static void set_screen_mode(unsigned char mode);

    unsigned char far *screen_memory;
    unsigned char far *screen_buffer;
};

#endif