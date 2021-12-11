#include "keyboard.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>
#include <iostream.h>

/*
 * Keyboard scanning + utils
 * http://www3.telus.net/alexander_russell/course/chapter_4.htm
 */

#define KEYBOARD_PORT 0x60
#define KEYBOARD_INT 0x9

#define KEY_UP_MASK 0x80
#define KEY_ALL_MASK 0x7F

namespace Keyboard
{
    void far __interrupt new_handler();
    void far __interrupt (*old_handler)();

    unsigned char far *pressed_mask;

    /* Circular queue for keyboard scan codes */
    unsigned char far *kb_scan_q;
    unsigned char kb_scan_temp;
    unsigned char kb_scan_head;
    unsigned char kb_scan_tail;

    void init_keyboard()
    {
        old_handler = _dos_getvect(KEYBOARD_INT);
        _dos_setvect(KEYBOARD_INT, new_handler);

        pressed_mask = new unsigned char[256];
        memset(pressed_mask, 255, 0x00);

        kb_scan_q = new unsigned char[256];

        kb_scan_temp = 0;
        kb_scan_head = 0;
        kb_scan_tail = 0;
    }

    void deinit_keyboard()
    {
        delete[] pressed_mask;
        delete[] kb_scan_q;
        _dos_setvect(KEYBOARD_INT, old_handler);
    }

    bool check_if_pressed(unsigned char scancode)
    {
        return pressed_mask[scancode] == 1;
    }

    unsigned char get_ascii(unsigned char scancode, bool shifted)
    {
        if (scancode == 0x1E && shifted)
            return 0x61;
        if (scancode == 0x1E && !shifted)
            return 0x41;

        return 0;
    }

    void far __interrupt new_handler()
    {
        kb_scan_temp = inp(0x60);

        kb_scan_q[kb_scan_tail++] = kb_scan_temp;

        if (kb_scan_temp != 0xE0) // ignore extended
        {
            if ((kb_scan_temp & KEY_UP_MASK))
            {
                pressed_mask[kb_scan_temp & KEY_ALL_MASK] = 0;
            }
            else
            {
                pressed_mask[kb_scan_temp & KEY_ALL_MASK] = 1;
            }
        }

        outp(0x20, 0x20);
    }

    bool get_keyevent(KeyEvent *keyevent)
    {
        unsigned char kb_scan;
        while (kb_scan_head != kb_scan_tail)
        {
            kb_scan = kb_scan_q[kb_scan_head++];

            /* Keypad keys may emit extra 0xe0 events - safe to ignore */
            if (kb_scan == 0xE0)
            {
                continue;
            }

            keyevent->key_code = kb_scan & KEY_ALL_MASK;
            keyevent->is_keydown = !(kb_scan & KEY_UP_MASK);

            return true;
        }
        return false;
    }
} // namespace Keyboard