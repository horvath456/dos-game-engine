#include "screen.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>

using namespace std;

Screen::Screen()
{
    screen_memory = (unsigned char far *)0xA0000000L;
    screen_buffer = new unsigned char[64000];
    set_screen_mode(0x13);
    clear_screen_buffer();
}

Screen::~Screen()
{
    delete[] screen_buffer;
    set_screen_mode(0x2);
}

unsigned char far *Screen::get_buffer(unsigned int x, unsigned int y)
{
    return screen_buffer + y * SCREEN_WIDTH + x;
}

void Screen::draw_screen_buffer()
{
    unsigned char far *src = screen_buffer;
    unsigned char far *dst = screen_memory;
    _asm {
        push ds
        push es
        lds si, [src]
        les di, [dst]
        mov ecx, 16000
        rep movsd
        pop es
        pop ds
    }
}

void Screen::clear_screen_buffer()
{
    unsigned char far *dst = screen_buffer;
    _asm {
        push ds
        push es
        les di, [dst]
        mov ecx, 16000
        xor eax, eax
        rep stosd
        pop es
        pop ds
    }
}

void Screen::draw_background_color(unsigned char c8)
{
    unsigned char far *dst = screen_buffer;
    unsigned long int color = ((c8 << 8 | c8) << 16) | (c8 << 8 | c8);
    _asm
    {
        push ds
        push es
        mov eax, color
        les di, [dst]
        mov ecx, 16000
        rep stosd
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
        lds si, [buf]
        les di, [dst]
        mov ecx, 16000
        rep movsd
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

void Screen::wait_retrace()
{
    _asm {
        push ax 
        push dx
        mov dx, 0x03DA
        Lbl1:
        in al, dx 
        test al, 1000b
        je Lbl1
        Lbl2:
        in al, dx 
        test al, 1000b
        je Lbl2
        pop dx 
        pop ax
    }
    return;
}