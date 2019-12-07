#include <fstream.h>

#include "screen.h"
#include "player.h"
#include "drawobj.h"

#include "level.h"

Level::Level(const char *filename, Player& pl)
{
    player = &pl;
    x = 0;
    y = 0;

   // draw_objects = new far DrawObject[2];
    //draw_objects[0] = DrawObject("IMAGES\\GRASS.SPR");

    fstream infile = fstream(filename, ios::in | ios::binary);

    unsigned char buf[2];

    infile.get(buf[0]);
    infile.get(buf[1]);

    cnt_level_elements = (buf[0] << 8) + buf[1];

    this->level_data = new far unsigned char[(unsigned int)(cnt_level_elements * 5)];

    for (unsigned int i = 0; i < cnt_level_elements * 5; i++)
    {
        unsigned char c;
        infile.get(c);
        this->level_data[i] = c;
    }
}

void Level::draw(Screen &screen)
{
}