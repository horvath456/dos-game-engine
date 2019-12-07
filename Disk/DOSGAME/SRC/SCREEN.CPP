#include "screen.h"
#include "farmem.h"

Screen::Screen()
{
    screen_memory = (unsigned char far *)0xA0000000L;
    screen_buffer = new far unsigned char[64000];
    clear_screen_buffer();
}

Screen::~Screen()
{
    delete[] screen_buffer;
}

void Screen::draw_screen_buffer()
{
    fmemcpy(screen_buffer, screen_memory, 64000);
}

void Screen::clear_screen_buffer()
{
    fmemset(screen_buffer, 64000, 0);
}

void Screen::plot_pixel(unsigned int x, unsigned int y, unsigned char color)
{
    screen_buffer[((y << 8) + (y << 6)) + x] = color;
}

void Screen::set_graphics_mode()
{
    asm {
        mov ah, 0x0
        mov al, 0x13
        int 0x10
    }
}