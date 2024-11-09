#include "sprite.h"

#include <fstream>
#include <iostream.h>
#include "sprsheet.h"

unsigned char Engine::Sprite::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}

Engine::Sprite::~Sprite()
{
    delete[] buffer;
}

Engine::Sprite::Sprite(Spritesheet &spritesheet, unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    width = w;
    height = h;
    buffer = new unsigned char[(unsigned int)(width * height)];

    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            buffer[i * width + j] = spritesheet.pixel_at(x + j, y + i);
        }
    }
}

Engine::Sprite::Sprite(const char *filename)
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

unsigned char far *Engine::Sprite::get_buffer()
{
    return buffer;
}