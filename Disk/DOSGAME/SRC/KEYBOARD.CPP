#include "keyboard.h"

#include <dos.h>
#include <iostream.h>
#include "farmem.h"

void Keyboard::init_keyboard()
{
    asm {
        mov ax, 0x0304
        int 0x16
    }
    old_handler = getvect(KEYBOARD_INT);
    setvect(KEYBOARD_INT, new_handler);

    pressed_mask = new unsigned char[256];
    fmemset(pressed_mask, 256, 0);
}

void Keyboard::deinit_keyboard()
{
    setvect(KEYBOARD_INT, old_handler);
    asm {
        mov ax, 0x0300
        int 0x16
    }
}

int Keyboard::check_if_pressed(unsigned char scancode)
{
    return pressed_mask[scancode & 0x1F];
}

unsigned char Keyboard::get_last_action()
{
    return last_action;
}

void Keyboard::clear_last_action()
{
    last_action = 0;
}

int Keyboard::get_key_down(unsigned char scancode)
{
    if (((get_last_action() & 0x1F) == (scancode & 0x1F)) && ((get_last_action() & 0x80) == 0))
    {
        clear_last_action();
        return 1;
    }
    else
        return 0;
}

int Keyboard::get_key_up(unsigned char scancode)
{
    if (((get_last_action() & 0x1F) == (scancode & 0x1F)) && ((get_last_action() & 0x80) == 1))
    {
        clear_last_action();
        return 1;
    }
    else
        return 0;
}

void far interrupt Keyboard::new_handler(...)
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

void far interrupt (*Keyboard::old_handler)(...);
unsigned char *Keyboard::pressed_mask;
unsigned char Keyboard::last_action;