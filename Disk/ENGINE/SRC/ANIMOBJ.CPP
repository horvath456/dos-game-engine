#include "animobj.h"

#include <fstream>
#include "screen.h"
#include "sprite.h"
#include "draw.h"

Engine::AnimationObject::AnimationObject(Sprite *sprites[], unsigned int num_sprites, int x, int y)
{
    frame = 0;

    num_frames = num_sprites;
    if (num_frames > 32)
    {
        num_frames = 32;
    }

    width = sprites[0]->width;
    height = sprites[0]->height;

    for (unsigned int i = 0; i < num_frames; i++)
    {
        frames[i] = sprites[i];
    }

    this->x = x;
    this->y = y;
}

unsigned char far *Engine::AnimationObject::get_buffer()
{
    return frames[frame]->get_buffer();
}

void Engine::AnimationObject::set_frame(unsigned int f)
{
    frame = f % num_frames;
}

void Engine::AnimationObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Engine::AnimationObject::draw(Screen &screen)
{
    Draw::draw_sprite_buffer(screen, get_buffer(), x, y, width, height, true);
}