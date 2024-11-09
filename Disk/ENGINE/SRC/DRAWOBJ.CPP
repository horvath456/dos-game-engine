#include "drawobj.h"

#include <fstream>
#include <iostream.h>
#include "screen.h"
#include "sprite.h"
#include "draw.h"

void Engine::DrawObject::draw(Screen &screen)
{
    Draw::draw_sprite_buffer(screen, buffer, x, y, width, height, transparent);
}

void Engine::DrawObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}

unsigned char Engine::DrawObject::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}

Engine::DrawObject::DrawObject(Sprite &sprite, int x, int y, bool transparent)
{
    this->transparent = transparent;
    width = sprite.width;
    height = sprite.height;
    buffer = sprite.get_buffer();
    loaded_from_file = false;
    this->x = x;
    this->y = y;
}

Engine::DrawObject::DrawObject(const char *filename, int x, int y, bool transparent)
{
    this->transparent = transparent;
    this->x = x;
    this->y = y;

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
    loaded_from_file = true;

    for (unsigned int i = 0; i < width * height; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }
}

Engine::DrawObject::~DrawObject()
{
    if (loaded_from_file)
        delete[] buffer;
}

unsigned char far *Engine::DrawObject::get_buffer()
{
    return buffer;
}