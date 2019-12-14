#include "font.h"

#include <fstream>
#include <iostream.h>

#define PSF2_MAGIC0 0x72
#define PSF2_MAGIC1 0xb5
#define PSF2_MAGIC2 0x4a
#define PSF2_MAGIC3 0x86

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

Font::Font(const char *filename)
{
    std::fstream infile = std::fstream(filename, std::ios::in | std::ios::binary);

    unsigned char buf[4];

    infile.get(buf[0]);
    infile.get(buf[1]);
    infile.get(buf[2]);
    infile.get(buf[3]);

    if (buf[0] == PSF1_MAGIC0 && buf[1] == PSF1_MAGIC1)
    {
        width = 8;
        height = buf[3];
        infile.seekg(4);
    }
    else if (buf[0] == PSF2_MAGIC0 && buf[1] == PSF2_MAGIC1 && buf[2] == PSF2_MAGIC2 && buf[3] == PSF2_MAGIC3)
    {
        infile.seekg(24);
        infile.get(height);
        infile.seekg(28);
        infile.get(width);
        infile.seekg(32);
    }
    else
    {
        return;
    }

    unsigned int buffer_len = (((width + 7) / 8) * height) * 256;

    buffer = new unsigned char[buffer_len];

    for (unsigned int i = 0; i < buffer_len; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }
}

Font::~Font()
{
    delete[] buffer;
}

unsigned char far *Font::get_buffer(char c)
{
    return buffer + (unsigned char)c * ((width + 7) / 8) * height;
}