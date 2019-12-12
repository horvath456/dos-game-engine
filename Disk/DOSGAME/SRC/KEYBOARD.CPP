#include "keyboard.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_INT 0x9

namespace Keyboard
{
void far __interrupt new_handler();

void far __interrupt (*old_handler)();
unsigned char *pressed_mask;
unsigned char last_action = 0x0;

void init_keyboard()
{
    _asm {
        mov ax, 0x0304
        int 0x16
    }
    old_handler = _dos_getvect(KEYBOARD_INT);
    _dos_setvect(KEYBOARD_INT, new_handler);

    pressed_mask = new unsigned char[256];
    memset(pressed_mask, 256, 0);
}

void deinit_keyboard()
{
    _dos_setvect(KEYBOARD_INT, old_handler);
    _asm {
        mov ax, 0x0300
        int 0x16
    }
}

int check_if_pressed(unsigned char scancode)
{
    return pressed_mask[scancode & 0x1F];
}

unsigned char get_last_action()
{
    return last_action;
}

void clear_last_action()
{
    last_action = 0;
}

int get_key_down(unsigned char scancode)
{
    if (((get_last_action() & 0x1F) == (scancode & 0x1F)) && ((get_last_action() & 0x80) == 0))
    {
        clear_last_action();
        return 1;
    }
    else
        return 0;
}

int get_key_up(unsigned char scancode)
{
    if (((get_last_action() & 0x1F) == (scancode & 0x1F)) && ((get_last_action() & 0x80) == 1))
    {
        clear_last_action();
        return 1;
    }
    else
        return 0;
}

void far __interrupt new_handler()
{
    unsigned char byte = inp(KEYBOARD_PORT);
    if ((byte & 0x80))
    {
        pressed_mask[byte & 0x3F] = 0;
        last_action = (byte & 0x3F) | 0x80;
    }
    else
    {
        if (pressed_mask[byte & 0x3F] == 0)
            last_action = (byte & 0x3F);
        pressed_mask[byte & 0x3F] = 1;
    }
    outp(0x20, 0x20);
}
} // namespace Keyboard