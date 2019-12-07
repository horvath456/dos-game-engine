#include "drawobj.h"
#include "screen.h"

#include <iostream.h>

void DrawObject::draw(Screen &screen, int x, int y)
{
    for (int i = 0; i < height; i++)
    {
        if (y + i < 0 || y + i >= 200)
            continue;
        for (int j = 0; j < width; j++)
        {
            if (x + j < 0 || x + j >= 320)
                continue;
            screen.plot_pixel(x + j, y + i, pixel_at(j, i));
        }
    }
}

unsigned char DrawObject::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}

DrawObject::~DrawObject()
{
    delete[] this->buffer;
}

DrawObject::DrawObject(const char *filename)
{
    fstream infile = fstream(filename, ios::in | ios::binary);

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

    this->buffer = new far unsigned char[(unsigned int)(width * height)];

    for (unsigned int i = 0; i < width * height; i++)
    {
        unsigned char c;
        infile.get(c);
        this->buffer[i] = c;
    }
}

DrawObject::DrawObject(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    this->buffer = new far unsigned char[width * height];
}

unsigned char far *DrawObject::get_buffer()
{
    return buffer;
}