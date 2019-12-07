#include <dos.h>
#include <iostream.h>
#include "farmem.h"

#include "keyboard.h"

Keyboard::Keyboard()
{
    init();
    buffer = new unsigned char[256];
    fmemset(buffer, 256, 0);
}

Keyboard::~Keyboard()
{
    deinit();
}

int Keyboard::check_if_pressed(unsigned char scancode)
{
    return buffer[scancode & 0x1F];
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
        buffer[byte & 0x1F] = 0;
    }
    else
    {
        buffer[byte & 0x1F] = 1;
    }
    outp(0x20, 0x20);
}

void far interrupt (*Keyboard::old_handler)(...);
unsigned char *Keyboard::buffer;