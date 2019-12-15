#include "joystick.h"

#include <conio.h>
#include <dos.h>
#include "timer.h"

#define DIVIDE_VALUE 100

namespace Joystick
{
struct JoystickCoordinates coordinates = {0, 0};
struct JoystickButtons buttons = {false, false};

unsigned long long game_time = 0;
unsigned long long game_time_bit0 = 0;
unsigned long long game_time_bit1 = 0;

int is_joystick_installed = -1;

void callback_handler()
{
    unsigned char state = inp(0x201);

    if ((state & 1) && game_time_bit0 == 0)
        game_time_bit0 = Timer::get_time();
    if ((state & 2) && game_time_bit1 == 0)
        game_time_bit1 = Timer::get_time();

    buttons.button1 = state & 16;
    buttons.button2 = state & 32;

    if (game_time_bit0 > 0 && game_time_bit1 > 0)
    {
        coordinates.x = (game_time_bit0 - game_time) / DIVIDE_VALUE;
        coordinates.y = (game_time_bit1 - game_time) / DIVIDE_VALUE;
        game_time_bit0 = 0;
        game_time_bit1 = 0;
        game_time = Timer::get_time();
        outp(0x201, 42);
    }
}

bool check_if_joystick_is_installed()
{
    if (is_joystick_installed == -1)
    {
        unsigned char switch_states = 0x0;
        unsigned int axis1 = 0x0;
        unsigned int axis2 = 0x0;
        unsigned int axis3 = 0x0;
        unsigned int axis4 = 0x0;
        _asm {
            mov ah, 0x84
            xor dx, dx
            int 0x15
            mov [switch_states],al
            mov ah, 0x84
            mov dx, 0x0001
            int 0x15
            mov [axis1], ax
            mov [axis2], bx
            mov [axis3], cx
            mov [axis4], dx
        }
        is_joystick_installed = switch_states == 0x0 && axis1 == 0x0 && axis2 == 0x0 && axis3 == 0x0 && axis4 == 0x0;
    }
    return is_joystick_installed;
}

void start_joystick_reading()
{
    game_time = Timer::get_time();
    outp(0x201, 42);
    Timer::add_callback(&callback_handler);
}

void stop_joystick_reading()
{
    Timer::remove_callback(&callback_handler);
}
} // namespace Joystick