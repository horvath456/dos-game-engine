#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 57
#define KEY_ESC 1
#define KEY_ENTER 0x1C
#define KEY_DEL 0x53
#define KEY_BACKSPACE 0x0E
#define KEY_LSHIFT 42
#define KEY_RSHIFT 54

namespace Keyboard
{
    typedef struct
    {
        unsigned char key_code;
        unsigned char ascii_code;
        bool is_keydown;
    } KeyEvent;

    void init_keyboard();
    void deinit_keyboard();

    void set_leds(bool caps_lock, bool num_lock, bool scroll_lock);

    bool check_if_pressed(unsigned char scancode);

    bool get_keyevent(KeyEvent *keyevent);
} // namespace Keyboard

#endif