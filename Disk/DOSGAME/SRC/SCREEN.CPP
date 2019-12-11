#include "screen.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>

//#define NO_DOUBLE_BUFFER

using namespace std;

Screen::Screen()
{
    screen_memory = (unsigned char far *)0xA0000000L;
#ifdef NO_DOUBLE_BUFFER
    screen_buffer = screen_memory;
#else
    screen_buffer = new unsigned char[64000];
#endif
    set_screen_mode(0x13);
    clear_screen_buffer();
}

Screen::~Screen()
{
#ifndef NO_DOUBLE_BUFFER
    delete[] screen_buffer;
#endif
    set_screen_mode(0x2);
}

unsigned char far *Screen::get_buffer(unsigned int x, unsigned int y)
{
    return screen_buffer + y * SCREEN_WIDTH + x;
}

void Screen::draw_screen_buffer()
{
#ifndef NO_DOUBLE_BUFFER
    unsigned char far *src = screen_buffer;
    unsigned char far *dst = screen_memory;
    _asm {
        push ds
        push es
        push cx
        push si 
        push di
        lds si, [src]
        les di, [dst]
        mov cx, 32000
        rep movsw
        pop di
        pop si
        pop cx
        pop es
        pop ds
    }
#endif
}

void Screen::clear_screen_buffer()
{
    unsigned char far *dst = screen_buffer;
    _asm {
        push ds
        push es
        push cx
        push si 
        push di
        les di, [dst]
        mov cx, 32000
        xor ax, ax
        rep stosw
        pop di
        pop si
        pop cx
        pop es
        pop ds
    }
}

void Screen::draw_background(unsigned char far *buf)
{
    unsigned char far *dst = screen_buffer;
    _asm {
        push ds
        push es
        push cx
        push si 
        push di
        lds si, [buf]
        les di, [dst]
        mov cx, 32000
        rep movsw
        pop di
        pop si
        pop cx
        pop es
        pop ds
    }
}

void Screen::plot_pixel(unsigned int x, unsigned int y, unsigned char color)
{
    screen_buffer[((y << 8) + (y << 6)) + x] = color;
}

void Screen::set_screen_mode(unsigned char mode)
{
    _asm {
        xor ah, ah
        mov al, mode
        int 0x10
    }
}

int Screen::is_in_vertical_retrace()
{
    return inp(0x3DA) & 0x8;
}