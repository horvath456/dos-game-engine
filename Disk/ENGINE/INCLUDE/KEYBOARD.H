#ifndef ENGINE_KEYBOARD_H
#define ENGINE_KEYBOARD_H

#pragma once

namespace Engine
{
    namespace Keyboard
    {
        const unsigned char KEY_UP = 72;
        const unsigned char KEY_DOWN = 80;
        const unsigned char KEY_LEFT = 75;
        const unsigned char KEY_RIGHT = 77;
        const unsigned char KEY_SPACE = 57;
        const unsigned char KEY_ESC = 1;
        const unsigned char KEY_ENTER = 0x1C;
        const unsigned char KEY_DEL = 0x53;
        const unsigned char KEY_BACKSPACE = 0x0E;
        const unsigned char KEY_LSHIFT = 42;
        const unsigned char KEY_RSHIFT = 54;

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
    };
};

#endif ENGINE_KEYBOARD_H