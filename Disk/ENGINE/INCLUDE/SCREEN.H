#ifndef ENGINE_SCREEN_H
#define ENGINE_SCREEN_H

#pragma once

#include "rectangl.h"

namespace Engine
{
    class Screen
    {
    public:
        Screen();
        ~Screen();

        void draw_screen_buffer();

        void clear_screen_buffer();

        // x >= 0 && y >= 0 && x < SCREEN_WIDTH && y < SCREEN_HEIGHT
        unsigned char far *get_buffer(unsigned int x, unsigned int y);

        void draw_background(unsigned char *buf);
        void draw_background_color(unsigned char col);

        void plot_pixel(unsigned int x, unsigned int y, unsigned char color);

        static int is_in_vertical_retrace();
        static void wait_retrace();

        static const unsigned int SCREEN_WIDTH = 320;
        static const unsigned int SCREEN_HEIGHT = 200;

        static const unsigned int HPIXELS = 320;
        static const unsigned int VPIXELS = 200;

        void update_dirty_rectangles_list(Rectangle dirty_rects[], unsigned int num_rects);
        void clear_dirty_rectangles_list();

    private:
        void _draw_screen_buffer();
        void _draw_background(unsigned char *buf);
        void _draw_background_color(unsigned char col);

        static void set_screen_mode(unsigned char mode);

        unsigned char far *get_screen_memory_at(unsigned int x, unsigned int y);

        Rectangle dirty_rectangles[100];
        unsigned int num_dirty_rectangles;

        unsigned char far *screen_memory;
        unsigned char far *screen_buffer;
    };
};

#endif // ENGINE_SCREEN_H