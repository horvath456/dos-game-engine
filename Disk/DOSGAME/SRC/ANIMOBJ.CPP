#include "animobj.h"

#include <fstream>
#include "screen.h"
#include "sprite.h"

AnimationObject::AnimationObject(Sprite *sprites[], unsigned int num_sprites, int x, int y)
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

unsigned char far *AnimationObject::get_buffer()
{
    return frames[frame]->get_buffer();
}

void AnimationObject::set_frame(unsigned int f)
{
    frame = f % num_frames;
}

void AnimationObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}

void AnimationObject::draw(Screen &screen)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    if (x <= ((int)width * (-1)) || y <= ((int)height * (-1)))
        return;

    unsigned char far *buf = get_buffer();

    unsigned int w = width;
    unsigned int h = height;

    unsigned int si_add_after = 0;
    unsigned int si_add_before = 0;

    if (x + (int)width > SCREEN_WIDTH)
    {
        w += (SCREEN_WIDTH - x - (int)width);
        si_add_after = width - w;
    }
    if (y + (int)height > SCREEN_HEIGHT)
    {
        h += (SCREEN_HEIGHT - y - (int)height);
    }
    if (x < 0)
    {
        w += x;
        x = 0;
        si_add_before = width - w;
    }
    if (y < 0)
    {
        h += y;
        y = 0;
        buf += width * (height - h);
    }

    unsigned int add_w = SCREEN_WIDTH - w;
    unsigned char far *screenbuf = screen.get_buffer(x, y);

    _asm {
        push es 
        push ds
        mov dx, h
        lds si, [buf]
        les di, [screenbuf]
    height_loop_1:
        add si, si_add_before
        mov cx, w
    width_loop_1:
        lodsb
        cmp al, 0xFF
        je cont_loop_1
        stosb
        dec di
    cont_loop_1:
        inc di
        dec cx
        jnz width_loop_1
        add si, si_add_after
        add di, add_w
        dec dx
        jnz height_loop_1
        pop ds 
        pop es
    }
}