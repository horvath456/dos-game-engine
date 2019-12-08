#include "keyboard.h"

#include <dos.h>
#include <iostream.h>
#include "farmem.h"

Keyboard::Keyboard()
{
    init();
    pressed_mask = new unsigned char[256];
    fmemset(pressed_mask, 256, 0);
}

Keyboard::~Keyboard()
{
    deinit();
}

int Keyboard::check_if_pressed(unsigned char scancode)
{
    return pressed_mask[scancode & 0x1F];
}

unsigned char Keyboard::get_last_action()
{
    unsigned char c = last_action;
    last_action = 0;
    return c;
}

void Keyboard::init()
{
    old_handler = getvect(KEYBOARD_INT);
    setvect(KEYBOARD_INT, new_handler);
}

void Keyboard::deinit()
{
    setvect(KEYBOARD_INT, old_handler);
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