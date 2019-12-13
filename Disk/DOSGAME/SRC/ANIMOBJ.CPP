#include "animobj.h"

#include <fstream>
#include "screen.h"
#include "drawobj.h"

AnimationObject::AnimationObject(const char *filename)
{
    frame = 0;

    std::fstream infile = std::fstream(filename, std::ios::in | std::ios::binary);

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
    unsigned int fr = frame;
    unsigned char far *screenbuf = screen.get_buffer(x, y);
    unsigned int w = width;
    unsigned int h = height;
    unsigned int add_w = SCREEN_WIDTH - width;
    unsigned char far *buf = get_buffer();
    _asm {
        push dx 
        push si 
        push di 
        push es 
        push ds 
        push cx 
        push ax

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

        pop ax 
        pop cx 
        pop ds 
        pop es 
        pop di 
        pop si 
        pop dx
    }
}