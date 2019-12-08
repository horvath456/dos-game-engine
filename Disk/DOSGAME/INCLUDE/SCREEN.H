#ifndef SCREEN_H
#define SCREEN_H

#pragma once

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

class Screen
{
public:
    Screen();
    ~Screen();

    void draw_screen_buffer();

    void clear_screen_buffer();

    // x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT
    unsigned char far* get_buffer(unsigned int x, unsigned int y);

    void draw_background(unsigned char far *buf);

    void plot_pixel(unsigned int x, unsigned int y, unsigned char color);

private:
    static void set_screen_mode(unsigned char mode);

    unsigned char far *screen_memory;
    unsigned char far *screen_buffer;
};

#endif