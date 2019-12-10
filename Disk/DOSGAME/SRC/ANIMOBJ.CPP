#include "animobj.h"

#include "screen.h"
#include "drawobj.h"

AnimationObject::AnimationObject(unsigned int numframes, const char *filename)
{
    frame = 0;
    num_frames = numframes;

    fstream infile = fstream(filename, ios::in | ios::binary);

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

    buffer = new far unsigned char[(unsigned int)(num_frames * width * height)];

    for (unsigned int i = 0; i < numframes * width * height; i++)
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

void AnimationObject::draw(Screen &screen, int x, int y)
{
    unsigned int fr = frame;
    unsigned char far *screenbuf = screen.get_buffer(x, y);
    unsigned int w = width;
    unsigned int h = height;
    unsigned int add_w = SCREEN_WIDTH - width;
    unsigned char far *buf = buf + ((w * h) * fr);
    asm {
            mov dx, h
            lds si, [buf]
            les di, [screenbuf]
    }
height_loop_1:
    asm {
            mov cx, w
    }
width_loop_1:
    asm {
            lodsb
            cmp al, 0xFF
            je cont_loop_1
            stosb
            dec di
    }
cont_loop_1:
    asm {
            inc di
            dec cx
            jnz width_loop_1
            add di, add_w
            dec dx
            jnz height_loop_1
    }
}