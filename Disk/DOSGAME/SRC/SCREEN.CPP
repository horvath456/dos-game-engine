#include "screen.h"
#include "farmem.h"

Screen::Screen()
{
    screen_memory = (unsigned char far *)0xA0000000L;
    screen_buffer = new far unsigned char[64000];
    clear_screen_buffer();
    set_screen_mode(0x13);
}

Screen::~Screen()
{
    delete[] screen_buffer;
    set_screen_mode(0x2);
}

void Screen::draw_screen_buffer()
{
    unsigned char far *src = screen_buffer;
    unsigned char far *dst = screen_memory;
    asm {
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
}

void Screen::clear_screen_buffer()
{
    unsigned char far *dst = screen_buffer;
    asm {
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
    asm {
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
    asm {
        xor ah, ah
        mov al, mode
        int 0x10
    }
}