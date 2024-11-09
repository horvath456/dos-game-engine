#include "sprsheet.h"

#include <fstream>
#include <iostream.h>

Engine::Spritesheet::~Spritesheet()
{
    delete[] buffer;
}

Engine::Spritesheet::Spritesheet(const char *filename)
{
    std::fstream infile(filename, std::ios::in | std::ios::binary);

    unsigned char buf[4];

    infile.get(buf[0]);
    infile.get(buf[1]);
    infile.get(buf[2]);
    infile.get(buf[3]);

    width = (buf[0] << 8) + buf[1];
    height = (buf[2] << 8) + buf[3];

    if (height > 200)
        height = 200;
    if (width > 320)
        width = 320;

    buffer = new unsigned char[(unsigned int)(width * height)];

    for (unsigned int i = 0; i < width * height; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }
}

unsigned char far *Engine::Spritesheet::get_buffer_at(unsigned int x, unsigned int y)
{
    return buffer + (y * width + x);
}

unsigned char Engine::Spritesheet::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}