#include "screen.h"

#include <dos.h>
#include <malloc.h>
#include <stdio>
#include <conio.h>
#include <string.h>
#include "rectangl.h"

using namespace std;

Screen::Screen()
{
    num_dirty_rectangles = 0;
    screen_memory = (unsigned char far *)0xA0000000L;
    screen_buffer = (unsigned char far *)_fmalloc(SCREEN_WIDTH * SCREEN_HEIGHT);
    set_screen_mode(0x13);
    clear_screen_buffer();
    draw_screen_buffer();
}

Screen::~Screen()
{
    _ffree(screen_buffer);
    set_screen_mode(0x2);
}

unsigned char far *Screen::get_buffer(unsigned int x, unsigned int y)
{
    return screen_buffer + y * SCREEN_WIDTH + x;
}

unsigned char far *Screen::get_screen_memory_at(unsigned int x, unsigned int y)
{
    return screen_memory + y * SCREEN_WIDTH + x;
}

void Screen::_draw_screen_buffer()
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

void Screen::draw_screen_buffer()
{
    if (num_dirty_rectangles == 0)
        return _draw_screen_buffer();

    for (unsigned int i = 0; i < num_dirty_rectangles; i++)
    {
        unsigned char far *src = get_buffer(dirty_rectangles[i].left, dirty_rectangles[i].top);
        unsigned char far *dst = get_screen_memory_at(dirty_rectangles[i].left, dirty_rectangles[i].top);
        unsigned int rect_width = dirty_rectangles[i].right - dirty_rectangles[i].left;
        unsigned int rect_height = dirty_rectangles[i].bottom - dirty_rectangles[i].top;
        for (unsigned int j = 0; j < rect_height; j++)
        {
            /* Copy a row */
            for (unsigned int k = 0; k < rect_width; k++)
            {
                *dst++ = *src++;
            }
            dst += SCREEN_WIDTH - rect_width;
            src += SCREEN_WIDTH - rect_width;
        }
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

void Screen::_draw_background_color(unsigned char c8)
{
    unsigned char far *dst = screen_buffer;
    unsigned long int color = ((c8 << 8 | c8) << 16) | (c8 << 8 | c8);
    _asm
    {
        push es
        mov eax, color
        les di, [dst]
        mov ecx, 16000
        rep stosd
        pop es
    }
}

void Screen::draw_background_color(unsigned char c8)
{
    if (num_dirty_rectangles == 0)
        return _draw_background_color(c8);

    for (unsigned int i = 0; i < num_dirty_rectangles; i++)
    {
        unsigned char far *dst = get_buffer(dirty_rectangles[i].left, dirty_rectangles[i].top);
        unsigned int rect_width = dirty_rectangles[i].right - dirty_rectangles[i].left;
        unsigned int rect_height = dirty_rectangles[i].bottom - dirty_rectangles[i].top;
        for (unsigned int j = 0; j < rect_height; j++)
        {

            _asm
                {
                push es
                mov al, c8
                les di, [dst]
                mov cx, rect_width
                rep stosb
                pop es
                }
            dst += SCREEN_WIDTH;
        }
    }
}

void Screen::_draw_background(unsigned char far *buf)
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

void Screen::draw_background(unsigned char far *buf)
{
    if (num_dirty_rectangles == 0)
        return _draw_background(buf);

    for (unsigned int i = 0; i < num_dirty_rectangles; i++)
    {
        unsigned char far *src = buf + dirty_rectangles[i].top * SCREEN_WIDTH + dirty_rectangles[i].left;
        unsigned char far *dst = get_buffer(dirty_rectangles[i].left, dirty_rectangles[i].top);
        unsigned int rect_width = dirty_rectangles[i].right - dirty_rectangles[i].left;
        unsigned int rect_height = dirty_rectangles[i].bottom - dirty_rectangles[i].top;
        for (unsigned int j = 0; j < rect_height; j++)
        {
            /* Copy a row */
            for (unsigned int k = 0; k < rect_width; k++)
            {
                *dst++ = *src++;
            }
            dst += SCREEN_WIDTH - rect_width;
            src += SCREEN_WIDTH - rect_width;
        }
    }
}

void Screen::update_dirty_rectangles_list(Rectangle rectangles[], unsigned int num_rects)
{
    memcpy(dirty_rectangles, rectangles, num_rects * sizeof(rectangles[0]));
    num_dirty_rectangles = num_rects;
}

void Screen::clear_dirty_rectangles_list()
{
    num_dirty_rectangles = 0;
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