#ifndef MOUSE_H
#define MOUSE_H

#pragma once

namespace Mouse
{
struct MouseCoordinates
{
    unsigned int x;
    unsigned int y;
};
struct MouseStatus
{
    struct MouseCoordinates coors;
    bool left_button;
    bool right_button;
};
struct MouseLastButtonData
{
    bool pressed;
    MouseCoordinates coors;
};

bool init_mouse();

struct MouseStatus get_position_and_button_status();

struct MouseLastButtonData get_last_left_button_down();
struct MouseLastButtonData get_last_left_button_up();
struct MouseLastButtonData get_last_right_button_down();
struct MouseLastButtonData get_last_right_button_up();

void show_hardware_cursor();
void hide_hardware_cursor();
} // namespace Mouse

#endif