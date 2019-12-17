#include "animobj.h"

#include <fstream>
#include "screen.h"
#include "drawobj.h"

AnimationObject::AnimationObject(const char *filename)
{
    frame = 0;

    std::fstream infile(filename, std::ios::in | std::ios::binary);

    unsigned char buf[6];

    infile.get(buf[0]);
    infile.get(buf[1]);
    infile.get(buf[2]);
    infile.get(buf[3]);
    infile.get(buf[4]);
    infile.get(buf[5]);

    num_frames = (buf[0] << 8) + buf[1];
    width = (buf[2] << 8) + buf[3];
    height = (buf[4] << 8) + buf[5];

    buffer = new unsigned char[(unsigned int)(num_frames * width * height)];

    for (unsigned int i = 0; i < num_frames * width * height; i++)
    {
        unsigned char c;
        infile.get(c);
        this->buffer[i] = c;
    }
}

AnimationObject::~AnimationObject()
{
    delete[] buffer;
}

unsigned char far *AnimationObject::get_buffer()
{
    return buffer + (frame * (width * height));
}

void AnimationObject::draw(Screen &screen, int x, int y)
{
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
        return;
    if (x <= ((int)width * (-1)) || y <= ((int)height * (-1)))
        return;

    unsigned char far *buf = buffer;

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