#include "mouse.h"

namespace Mouse
{
bool init_mouse()
{
    unsigned int *IVT = (unsigned int *)0x0;
    unsigned int offset = IVT[0x33 * 2];
    unsigned int segment = IVT[0x33 * 2 + 2];
    if (offset == 0x0 && segment == 0x0)
        return false;

    unsigned int status = 0x0000;

    _asm {
        push ax 
        xor ax, ax
        int 0x33
        mov [status], ax
        pop ax
    }

    return status == 0xFFFF;
}

void show_hardware_cursor()
{
    _asm {
        push ax
        mov ax, 0x0001
        int 0x33
        pop ax
    }
}

void hide_hardware_cursor()
{
    _asm {
        push ax
        mov ax, 0x0002
        int 0x33
        pop ax
    }
}

struct MouseStatus get_position_and_button_status()
{
    struct MouseStatus status;

    unsigned int buttons_status;
    unsigned int x;
    unsigned int y;

    _asm {
        push ax 
        push bx 
        push cx 
        push dx
        mov ax, 0x0003
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [buttons_status], bx
        pop dx 
        pop cx 
        pop bx 
        pop ax
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
        push ax 
        push bx 
        push cx 
        push dx
        mov ax, 0x0005
        mov bx, 0x0000
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
        pop dx 
        pop cx 
        pop bx 
        pop ax
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
        push ax 
        push bx 
        push cx 
        push dx
        mov ax, 0x0006
        mov bx, 0x0000
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
        pop dx 
        pop cx 
        pop bx 
        pop ax
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
        push ax 
        push bx 
        push cx 
        push dx
        mov ax, 0x0005
        mov bx, 0x0001
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
        pop dx 
        pop cx 
        pop bx 
        pop ax
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
        push ax 
        push bx 
        push cx 
        push dx
        mov ax, 0x0006
        mov bx, 0x0001
        int 0x33
        mov [x], cx
        mov [y], dx
        mov [last_press_cnt], bx
        pop dx 
        pop cx 
        pop bx 
        pop ax
    }

    status.pressed = last_press_cnt > 0;
    status.coors.x = x;
    status.coors.y = y;

    return status;
}
} // namespace Mouse