#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

#define KEYBOARD_PORT 0x60
#define KEYBOARD_INT 0x9

class Keyboard
{
public:
    Keyboard();
    ~Keyboard();

    int check_if_pressed(unsigned char scancode);

private:
    static void far interrupt (*old_handler)(...);

    static void far interrupt new_handler(...);

    static void init();
    static void deinit();

    static unsigned char* buffer;
};

#endif