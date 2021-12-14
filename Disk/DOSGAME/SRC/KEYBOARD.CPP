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

    unsigned char get_ascii(unsigned char scancode, bool shifted);
    void kbd_ack();

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

    void kbd_ack()
    {
        _asm {
            kbd_wait:
            in     al,64h
            test     al,1
            jz     .ok
            in     al,60h
            jmp     kbd_wait
            .ok:
            test     al,2
            jnz     kbd_wait
        }
    }

    void set_leds(bool caps_lock, bool num_lock, bool scroll_lock)
    {
        unsigned char ledstatus = 0x0;
        if (caps_lock)
            ledstatus |= 0x1 << 2;
        if (num_lock)
            ledstatus |= 0x1 << 1;
        if (scroll_lock)
            ledstatus |= 0x1 << 0;
        outp(0x60, 0xed);
        kbd_ack();
        outp(0x60, ledstatus);
    }

    bool check_if_pressed(unsigned char scancode)
    {
        return pressed_mask[scancode] == 1;
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
            keyevent->ascii_code = get_ascii(kb_scan & KEY_ALL_MASK, check_if_pressed(KEY_LSHIFT) || check_if_pressed(KEY_RSHIFT));

            return true;
        }
        return false;
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

    unsigned char get_ascii(unsigned char scancode, bool shifted)
    {
        // A - Z
        if (scancode == 0x1E)
        {
            return shifted ? 0x41 : 0x61;
        }
        if (scancode == 0x30)
        {
            return shifted ? 0x42 : 0x62;
        }
        if (scancode == 0x2E)
        {
            return shifted ? 0x43 : 0x63;
        }
        if (scancode == 0x20)
        {
            return shifted ? 0x44 : 0x64;
        }
        if (scancode == 0x12)
        {
            return shifted ? 0x45 : 0x65;
        }
        if (scancode == 0x21)
        {
            return shifted ? 0x46 : 0x66;
        }
        if (scancode == 0x22)
        {
            return shifted ? 0x47 : 0x67;
        }
        if (scancode == 0x23)
        {
            return shifted ? 0x48 : 0x68;
        }
        if (scancode == 0x17)
        {
            return shifted ? 0x49 : 0x69;
        }
        if (scancode == 0x24)
        {
            return shifted ? 0x4A : 0x6A;
        }
        if (scancode == 0x25)
        {
            return shifted ? 0x4B : 0x6B;
        }
        if (scancode == 0x26)
        {
            return shifted ? 0x4C : 0x6C;
        }
        if (scancode == 0x32)
        {
            return shifted ? 0x4D : 0x6D;
        }
        if (scancode == 0x31)
        {
            return shifted ? 0x4E : 0x6E;
        }
        if (scancode == 0x18)
        {
            return shifted ? 0x4F : 0x6F;
        }
        if (scancode == 0x19)
        {
            return shifted ? 0x50 : 0x70;
        }
        if (scancode == 0x10)
        {
            return shifted ? 0x51 : 0x71;
        }
        if (scancode == 0x13)
        {
            return shifted ? 0x52 : 0x72;
        }
        if (scancode == 0x1F)
        {
            return shifted ? 0x53 : 0x73;
        }
        if (scancode == 0x14)
        {
            return shifted ? 0x54 : 0x74;
        }
        if (scancode == 0x16)
        {
            return shifted ? 0x55 : 0x75;
        }
        if (scancode == 0x2F)
        {
            return shifted ? 0x56 : 0x76;
        }
        if (scancode == 0x11)
        {
            return shifted ? 0x57 : 0x77;
        }
        if (scancode == 0x2D)
        {
            return shifted ? 0x58 : 0x78;
        }
        if (scancode == 0x15)
        {
            return shifted ? 0x59 : 0x79;
        }
        if (scancode == 0x2C)
        {
            return shifted ? 0x5A : 0x7A;
        }

        // Numbers
        if (scancode == 0x02)
        {
            return shifted ? 0x21 : 0x31;
        }
        if (scancode == 0x03)
        {
            return shifted ? 0x40 : 0x32;
        }
        if (scancode == 0x04)
        {
            return shifted ? 0x23 : 0x33;
        }
        if (scancode == 0x05)
        {
            return shifted ? 0x24 : 0x34;
        }
        if (scancode == 0x06)
        {
            return shifted ? 0x25 : 0x35;
        }
        if (scancode == 0x07)
        {
            return shifted ? 0x5E : 0x36;
        }
        if (scancode == 0x08)
        {
            return shifted ? 0x26 : 0x37;
        }
        if (scancode == 0x09)
        {
            return shifted ? 0x2A : 0x38;
        }
        if (scancode == 0x0A)
        {
            return shifted ? 0x28 : 0x39;
        }
        if (scancode == 0x0B)
        {
            return shifted ? 0x29 : 0x30;
        }

        // Special chars
        if (scancode == 0x0C)
        {
            return shifted ? 0x5F : 0x2D;
        }
        if (scancode == 0x0D)
        {
            return shifted ? 0x2B : 0x3D;
        }
        if (scancode == 0x1A)
        {
            return shifted ? 0x7B : 0x5B;
        }
        if (scancode == 0x1B)
        {
            return shifted ? 0x7D : 0x5D;
        }
        if (scancode == 0x27)
        {
            return shifted ? 0x3A : 0x3B;
        }
        if (scancode == 0x28)
        {
            return shifted ? 0x00 : 0x27;
        }
        if (scancode == 0x29)
        {
            return shifted ? 0x00 : 0x60;
        }
        if (scancode == 0x2B)
        {
            return shifted ? 0x7C : 0x5C;
        }
        if (scancode == 0x33)
        {
            return shifted ? 0x00 : 0x2C;
        }
        if (scancode == 0x34)
        {
            return shifted ? 0x00 : 0x2E;
        }
        if (scancode == 0x35)
        {
            return shifted ? 0x00 : 0x2F;
        }

        if (scancode == 0x0E) // backsp
        {
            return 0x08;
        }
        if (scancode == 0x1C) // enter
        {
            return 0x0D;
        }
        if (scancode == 0x53) // del
        {
            return shifted ? 0x2E : 0x00;
        }
        if (scancode == 0x39) // space
        {
            return 0x20;
        }

        return 0x00;
    }
} // namespace Keyboard