#ifndef JOYSTICK_H
#define JOYSTICK_H

#pragma once

namespace Joystick
{
struct JoystickCoordinates
{
    unsigned int x;
    unsigned int y;
};
struct JoystickButtons
{
    bool button1;
    bool button2;
};

bool check_if_joystick_is_installed();

void start_joystick_reading();
void stop_joystick_reading();

struct JoystickCoordinates coordinates = {0, 0};
struct JoystickButtons buttons = {false,false};
} // namespace Joystick

#endif