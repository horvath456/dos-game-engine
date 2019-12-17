#include "palette.h"

#include <fstream>
#include <conio.h>
#include "screen.h"
#include "timer.h"

Palette::Palette()
{
    palette = new unsigned char[768];
}

Palette::~Palette()
{
    delete[] palette;
}

void Palette::load_palette_file(const char *filename)
{
    std::fstream infile(filename, std::ios::in | std::ios::binary);

    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned char r, g, b;
        infile.get(r);
        infile.get(g);
        infile.get(b);

        palette[i * 3] = r;
        palette[i * 3 + 1] = g;
        palette[i * 3 + 2] = b;
    }
}

void Palette::load_palette()
{
    while (!Screen::is_in_vertical_retrace())
        ;
    for (unsigned int i = 0; i < 256; i++)
    {
        set_pal(i, palette[i * 3], palette[i * 3 + 1], palette[i * 3 + 2]);
    }
}

void Palette::blackout_palette()
{
    while (!Screen::is_in_vertical_retrace())
        ;
    for (unsigned int i = 0; i < 256; i++)
    {
        set_pal(i, 0x0, 0x0, 0x0);
    }
}

void Palette::get_pal(unsigned char i, unsigned char &r, unsigned char &g, unsigned char &b)
{
    outp(0x3C7, i);
    r = inp(0x3C9);
    g = inp(0x3C9);
    b = inp(0x3C9);
}

void Palette::set_pal(unsigned char i, unsigned char r, unsigned char g, unsigned char b)
{
    outp(0x3C8, i);
    outp(0x3C9, r);
    outp(0x3C9, g);
    outp(0x3C9, b);
}

void Palette::fade_in(unsigned int ms)
{
    for (int i = 1; i <= 64; i++)
    {
        if (ms > 0)
            Timer::delay(ms);
        while (!Screen::is_in_vertical_retrace())
            ;
        for (int j = 0; j < 256; j++)
        {
            unsigned char r, g, b;
            get_pal(j, r, g, b);
            if (r < palette[j * 3])
                r++;
            if (g < palette[j * 3 + 1])
                g++;
            if (b < palette[j * 3 + 2])
                b++;
            set_pal(j, r, g, b);
        }
    }
}

void Palette::fade_out(unsigned int ms)
{
    for (int i = 1; i <= 64; i++)
    {
        if (ms > 0)
            Timer::delay(ms);
        while (!Screen::is_in_vertical_retrace())
            ;
        for (int j = 0; j < 256; j++)
        {
            unsigned char r, g, b;
            get_pal(i, r, g, b);
            if (palette[j * 3] > 0)
                r--;
            if (palette[j * 3 + 1] > 0)
                g--;
            if (palette[j * 3 + 2] > 0)
                b--;
            set_pal(i, r, g, b);
        }
    }
}