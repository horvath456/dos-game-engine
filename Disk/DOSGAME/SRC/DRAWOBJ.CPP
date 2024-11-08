#include "drawobj.h"

#include <fstream>
#include <iostream.h>
#include "screen.h"
#include "sprite.h"

void DrawObject::draw(Screen &screen)
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

    if (x + (int)width >= SCREEN_WIDTH)
    {
        w += (SCREEN_WIDTH - x - (int)width);
        si_add_after = width - w;
    }
    if (y + (int)height >= SCREEN_HEIGHT)
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
    }

    if (transparent)
    {
        _asm {
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
        }
    }
    else if (w % 4 == 0)
    {
        w >>= 2;
        _asm {
            mov dx, h
            lds si, [buf]
            les di, [screenbuf]
        height_loop_2:
            add si, si_add_before
            xor ecx, ecx
            mov cx, w
            rep movsd
            add si, si_add_after
            add di, add_w
            dec dx
            jnz height_loop_2
        }
    }
    else
    {
        _asm {
            mov dx, h
            lds si, [buf]
            les di, [screenbuf]
        height_loop_2:
            add si, si_add_before
            mov cx, w
            rep movsb
            add si, si_add_after
            add di, add_w
            dec dx
            jnz height_loop_2
        }
    }

    _asm {
        pop ds 
        pop es
    }
}

void DrawObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}

unsigned char DrawObject::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}

DrawObject::DrawObject(Sprite &sprite, int x, int y, bool transparent)
{
    this->transparent = transparent;
    width = sprite.width;
    height = sprite.height;
    buffer = sprite.get_buffer();
    loaded_from_file = false;
    this->x = x;
    this->y = y;
}

DrawObject::DrawObject(const char *filename, int x, int y, bool transparent)
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

DrawObject::~DrawObject()
{
    if (loaded_from_file)
        delete[] buffer;
}

unsigned char far *DrawObject::get_buffer()
{
    return buffer;
}