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
    fmemcpy(screen_buffer, screen_memory, 64000);
}

void Screen::clear_screen_buffer()
{
    fmemset(screen_buffer, 64000, 0);
}

void Screen::draw_background(unsigned char far* buf)
{
    fmemcpy(buf, screen_buffer, 64000);
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