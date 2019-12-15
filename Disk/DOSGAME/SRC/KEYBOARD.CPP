#include "keyboard.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>
#include <iostream.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_INT 0x9

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 57
#define KEY_ESC 1

namespace Keyboard
{
void far __interrupt new_handler();

void far __interrupt (*old_handler)();
unsigned char *pressed_mask;
unsigned char last_action = 0x0;

void init_keyboard()
{
    old_handler = _dos_getvect(KEYBOARD_INT);
    _dos_setvect(KEYBOARD_INT, new_handler);

    pressed_mask = new unsigned char[256];
    memset(pressed_mask, 256, 0x00);
}

void deinit_keyboard()
{
    _dos_setvect(KEYBOARD_INT, old_handler);
}

bool check_if_pressed(unsigned char scancode)
{
    return pressed_mask[scancode] == 1;
}

unsigned char get_last_action()
{
    return last_action;
}

void clear_last_action()
{
    last_action = 0;
}

bool get_key_down(unsigned char scancode)
{
    if (((last_action & 0x7F) == scancode) && ((last_action & 0x80) == 0x0))
    {
        clear_last_action();
        return true;
    }
    else
        return false;
}

bool get_key_up(unsigned char scancode)
{
    if (((last_action & 0x7F) == scancode) && ((last_action & 0x80) == 0x80))
    {
        clear_last_action();
        return true;
    }
    else
        return false;
}

void far __interrupt new_handler()
{
    unsigned char byte = inp(KEYBOARD_PORT);
    if (byte != 0xE0)
    {
        if ((byte & 0x80))
        {
            pressed_mask[byte & 0x7F] = 0;
            last_action = (byte & 0x7F) | 0x80;
        }
        else
        {
            if (pressed_mask[byte & 0x7F] == 0)
                last_action = (byte & 0x7F);
            pressed_mask[byte & 0x7F] = 1;
        }
    }
    outp(0x20, 0x20);
}
} // namespace Keyboard