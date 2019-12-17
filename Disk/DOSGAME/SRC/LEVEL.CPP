#include "level.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream.h>
#include "screen.h"
#include "drawobj.h"
#include "player.h"

#define TILE_WIDTH 16.0
#define TILE_HEIGHT 16.0

unsigned char Level::get_tile(unsigned int x_tile, unsigned int y_tile)
{
    return buffer[y_tile * level_width + x_tile];
}

Level::Level(const char *filename)
{
    std::fstream infile(filename, std::ios::in | std::ios::binary);

    unsigned char buf[4];

    infile.get(buf[0]);
    infile.get(buf[1]);
    infile.get(buf[2]);
    infile.get(buf[3]);

    level_width = (buf[0] << 8) + buf[1];
    level_height = (buf[2] << 8) + buf[3];

    buffer = new unsigned char[(unsigned int)(level_width * level_height)];

    for (unsigned long int i = 0; i < level_width * level_height; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }

    camera_x = 0;
    camera_y = -8;
}

Level::~Level()
{
    delete[] buffer;
}

void Level::add_tile(DrawObject *d)
{
    tilemap.push_back((void *)d);
}

void Level::render(Screen &screen, Player &player)
{
    if (player.pos_x > 150)
    {
        if (player.pos_x < level_width * TILE_WIDTH - 150)
            camera_x = 150 - (player.pos_x);
        else
            camera_x = level_width * TILE_WIDTH - 150;
    }
    else
        camera_x = 0;

    for (unsigned int i = 0; i < level_height; i++)
    {
        for (unsigned int j = 0; j < level_width; j++)
        {
            unsigned char tile = buffer[i * level_width + j];

            if (tile == 0)
                continue;

            DrawObject *d = (DrawObject *)tilemap[tile - 1];
            d->draw(screen, camera_x + j * d->width, camera_y + i * d->height);
        }
    }

    player.anim_obj->draw(screen, player.pos_x > 150 ? 150 : player.pos_x + camera_x, player.pos_y + camera_y);
}