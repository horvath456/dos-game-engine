#ifndef ENGINE_PALETTE_H
#define ENGINE_PALETTE_H

#pragma once

namespace Engine
{
    class Palette
    {
    public:
        Palette();
        ~Palette();
        void load_palette_file(const char *filename);
        void load_palette();
        void blackout_palette();
        void fade_in(unsigned int ms = 0);
        void fade_out(unsigned int ms = 0);

    private:
        static void get_pal(unsigned char i, unsigned char &r, unsigned char &g, unsigned char &b);
        void set_pal(unsigned char i, unsigned char r, unsigned char g, unsigned char b);
        unsigned char far *palette;
    };
};

#endif // ENGINE_PALETTE_H