#include "palette.h"

#include <fstream.h>
#include <dos.h>

void Palette::load_palette(const char *filename)
{
    fstream infile = fstream(filename, ios::in | ios::binary);

    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned char r, g, b;
        infile.get(r);
        infile.get(g);
        infile.get(b);

        outp(0x3c8, (unsigned char)i);
        outp(0x3c9, r);
        outp(0x3c9, g);
        outp(0x3c9, b);
    }
}