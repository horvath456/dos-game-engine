#include "mouse.h"

#include <dos.h>
#include "screen.h"

#define MAX_MOUSE_EVENTS 10
#define EVENT_MASK 31

namespace Mouse
{
    void __far mouse_event_func();

    struct MouseStatus current_status; // gets updated on HW int

    MouseEvent mouse_events[MAX_MOUSE_EVENTS];
    int head = 0;
    int tail = 0;

    bool init_mouse()
    {
        unsigned int *IVT = (unsigned int *)0x0;
        unsigned int offset = IVT[0x66];
        unsigned int segment = IVT[0x68];
        if (offset == 0x0 && segment == 0x0)
            return false;

        unsigned int status = 0x0000;

        _asm {
            xor ax, ax
            int 0x33
            mov status, ax
        }

        if (status != 0xFFFF) return false;

        // set mouse x, y limits
        _asm {
            mov ax, 0x0007
            xor cx, cx
            mov dx, SCREEN_WIDTH - 1
            int 0x33
            mov ax, 0x0008
            xor cx, cx
            mov dx, SCREEN_HEIGHT - 1
            int 0x33
        }

        /* set initial mouse_x, mouse_y */
        unsigned int mouse_x, mouse_y;
        _asm {
            mov ax, 0x03
            int 0x33
            mov mouse_x, cx
            mov mouse_y, dx
        }
        current_status.coors.x = mouse_x;
        current_status.coors.y = mouse_y;
        current_status.left_button = false;
        current_status.right_button = false;

        unsigned int func_seg = (unsigned int)FP_SEG(mouse_event_func);
        unsigned int func_off = (unsigned int)FP_OFF(mouse_event_func);
        _asm
        {
            mov ax, func_seg
            mov es, ax
            mov dx, func_off
            mov ax, 0x0C 
            mov cx, EVENT_MASK 
            int 0x33
        }

        return true;
    }

    void show_hardware_cursor()
    {
        _asm {
            mov ax, 0x0001
            int 0x33
        }
    }

    void hide_hardware_cursor()
    {
        _asm {
            mov ax, 0x0002
            int 0x33
        }
    }

    struct MouseStatus get_current_status()
    {
        return current_status;
    }

    void update_position_and_button_status()
    {
        unsigned int buttons_status;
        unsigned int x;
        unsigned int y;

        _asm {
            mov ax, 0x0003
            int 0x33
            mov x, cx
            mov y, dx
            mov buttons_status, bx
        }

        current_status.left_button = buttons_status & 0x1;
        current_status.right_button = buttons_status & 0x2;
        current_status.coors.x = x;
        current_status.coors.y = y;
    }

    bool get_mouse_click_event(MouseEvent *mouseevent)
    {
        MouseEvent event;
        while (head != tail)
        {
            event = mouse_events[head++];

            mouseevent->coors.x = event.coors.x;
            mouseevent->coors.y = event.coors.y;
            mouseevent->type = event.type;

            if (head >= MAX_MOUSE_EVENTS)
                head = 0;

            return true;
        }
        return false;
    }

    void __far mouse_event_func()
    {
        unsigned int mouse_x, mouse_y;
        unsigned int buttons_status = 0;
        unsigned int flags;

        /* save info returned by mouse device driver */
        _asm {
         mov   flags,   ax
         mov   mouse_x, cx
         mov   mouse_y, dx
         mov   buttons_status, bx
        }

        if (((buttons_status & 0x1) && !current_status.left_button) ||  // left mousedown
            (!(buttons_status & 0x1) && current_status.left_button) ||  // left mouseup
            ((buttons_status & 0x2) && !current_status.right_button) || // right mousedown
            (!(buttons_status & 0x2) && current_status.right_button))   // right mouseup
        {
            // place the mouse event in a circular queue
            mouse_events[tail].coors.x = mouse_x;
            mouse_events[tail].coors.y = mouse_y;

            if ((buttons_status & 0x2) && !current_status.right_button)
                mouse_events[tail].type = right_mouse_down;

            if (!(buttons_status & 0x2) && current_status.right_button)
                mouse_events[tail].type = right_mouse_up;

            if ((buttons_status & 0x1) && !current_status.left_button)
                mouse_events[tail].type = left_mouse_down;

            if (!(buttons_status & 0x1) && current_status.left_button)
                mouse_events[tail].type = left_mouse_up;

            tail++;
            if (tail == MAX_MOUSE_EVENTS)
                tail = 0;
        }

        // update current_status
        current_status.coors.x = mouse_x;
        current_status.coors.y = mouse_y;
        current_status.left_button = buttons_status & 0x1;
        current_status.right_button = buttons_status & 0x2;
    }
} // namespace Mouse