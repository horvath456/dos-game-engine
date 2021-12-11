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
    unsigned char *get_buffer(unsigned int x, unsigned int y);

    void draw_background(unsigned char *buf);
    void draw_background_color(unsigned char col);

    void plot_pixel(unsigned int x, unsigned int y, unsigned char color);

    static int is_in_vertical_retrace();
    static void wait_retrace();

    static const unsigned int HPIXELS = SCREEN_WIDTH;
    static const unsigned int VPIXELS = SCREEN_HEIGHT;

private:
    static void set_screen_mode(unsigned char mode);

    unsigned char *screen_memory;
    unsigned char *screen_buffer;
};

#endif