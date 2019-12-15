#include "mouse.h"

#include "screen.h"

namespace Mouse
{
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
        mov [status], ax
    }

    if (status == 0xFFFF)
    {
        _asm {
            mov ax, 0x0007
            xor cx, cx
            mov dx, SCREEN_WIDTH-1
            int 0x33
            mov ax, 0x0008
            xor cx, cx
            mov dx, SCREEN_HEIGHT-1
            int 0x33
        }
        return true;
    }
    else return false;
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

struct MouseStatus get_position_and_button_status()
{
    struct MouseStatus status;

    unsigned int buttons_status;
    unsigned int x;
    unsigned int y;

    _asm {
        mov ax, 0x0003
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [buttons_status], bx
    }

    status.left_button = buttons_status & 0x1;
    status.right_button = buttons_status & 0x2;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}

struct MouseLastButtonData get_last_left_button_down()
{
    struct MouseLastButtonData status;

    unsigned int last_press_cnt = 0;
    unsigned int x;
    unsigned int y;

    _asm {
        mov ax, 0x0005
        mov bx, 0x0000
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
    }

    status.pressed = last_press_cnt > 0;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}

struct MouseLastButtonData get_last_left_button_up()
{
    struct MouseLastButtonData status;

    unsigned int last_press_cnt = 0;
    unsigned int x;
    unsigned int y;

    _asm {
        mov ax, 0x0006
        mov bx, 0x0000
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
    }

    status.pressed = last_press_cnt > 0;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}

struct MouseLastButtonData get_last_right_button_down()
{
    struct MouseLastButtonData status;

    unsigned int last_press_cnt = 0;
    unsigned int x;
    unsigned int y;

    _asm {
        mov ax, 0x0005
        mov bx, 0x0001
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
    }

    status.pressed = last_press_cnt > 0;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}

struct MouseLastButtonData get_last_right_button_up()
{
    struct MouseLastButtonData status;

    unsigned int last_press_cnt = 0;
    unsigned int x;
    unsigned int y;

    _asm {
        mov ax, 0x0006
        mov bx, 0x0001
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
    }

    status.pressed = last_press_cnt > 0;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}
} // namespace Mouse