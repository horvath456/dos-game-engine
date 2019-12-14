#include "level.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream.h>
#include "screen.h"
#include "drawobj.h"

Level::Level(const char *filename)
{
    std::fstream infile = std::fstream(filename, std::ios::in | std::ios::binary);

    unsigned char buf[4];

    infile.get(buf[0]);
    infile.get(buf[1]);
    infile.get(buf[2]);
    infile.get(buf[3]);

    width = (buf[0] << 8) + buf[1];
    height = (buf[2] << 8) + buf[3];

    buffer = new unsigned char[(unsigned int)(width * height)];

    for (unsigned long int i = 0; i < width * height; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }

    x = 0;
    y = -8;
}

Level::~Level()
{
    delete[] buffer;
}

void Level::add_tile(DrawObject *d)
{
    tilemap.push_back((void *)d);
}

void Level::draw(Screen &screen)
{
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            unsigned char tile = buffer[i * width + j];

            if (tile == 0)
                continue;

            DrawObject *d = (DrawObject *)tilemap[tile - 1];
            d->draw(screen, x + j * d->width, y + i * d->height);
        }
    }
}