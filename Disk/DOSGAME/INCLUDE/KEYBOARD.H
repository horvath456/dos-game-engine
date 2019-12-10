#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

#define KEYBOARD_PORT 0x60
#define KEYBOARD_INT 0x9

class Keyboard
{
public:
    static void init_keyboard();
    static void deinit_keyboard();

    static int check_if_pressed(unsigned char scancode);

    static unsigned char get_last_action();
    static void clear_last_action();

    static int get_key_up(unsigned char scancode);
    static int get_key_down(unsigned char scancode);

private:
    static void far interrupt (*old_handler)(...);

    static void far interrupt new_handler(...);

    static unsigned char *pressed_mask;
    static unsigned char last_action;
};

#endif