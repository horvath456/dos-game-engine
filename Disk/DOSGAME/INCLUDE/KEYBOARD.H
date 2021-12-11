#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 57
#define KEY_ESC 1

namespace Keyboard
{
    typedef struct
    {
        unsigned char key_code;
        bool is_keydown;
    } KeyEvent;

    void init_keyboard();
    void deinit_keyboard();

    bool check_if_pressed(unsigned char scancode);

    bool get_keyevent(KeyEvent *keyevent);
} // namespace Keyboard

#endif