#include "drawobj.h"

#include <fstream>
#include "screen.h"

void DrawObject::draw(Screen &screen, int x, int y, bool transparent)
{
    unsigned char far *buf = buffer;
    unsigned int w = width;
    unsigned int h = height;
    unsigned int add_w = SCREEN_WIDTH - width;
    unsigned char far *screenbuf = screen.get_buffer(x, y);

    _asm {
        push si 
        push di 
        push es 
        push ds
        push cx 
        push dx 
        push ax
    }

    if (transparent)
    {
        _asm {
            mov dx, h
            lds si, [buf]
            les di, [screenbuf]
        height_loop_1:
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
            add di, add_w
            dec dx
            jnz height_loop_1
        }
    }
    else
    {
        _asm {
            mov dx, h
            lds si, [buf]
            les di, [screenbuf]
        height_loop_2:
            mov cx, w
            rep movsb
            add di, add_w
            dec dx
            jnz height_loop_2
        }
    }

    _asm {
        pop ax 
        pop dx 
        pop cx
        pop ds 
        pop es 
        pop di 
        pop si
    }
}

unsigned char DrawObject::pixel_at(unsigned int x, unsigned int y)
{
    return buffer[y * width + x];
}

DrawObject::~DrawObject()
{
    delete[] buffer;
}

DrawObject::DrawObject(const char *filename)
{
    std::fstream infile = std::fstream(filename, std::ios::in | std::ios::binary);

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

unsigned char far *DrawObject::get_buffer()
{
    return buffer;
}