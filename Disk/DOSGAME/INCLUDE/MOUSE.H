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

    enum MouseEventType
    {
        left_mouse_down,
        left_mouse_up,
        right_mouse_down,
        right_mouse_up
    };

    struct MouseEvent
    {
        struct MouseCoordinates coors;
        MouseEventType type;
    };

    bool init_mouse();

    struct MouseStatus get_current_status();
    void update_position_and_button_status(); // uses int 0x33 call

    bool get_mouse_click_event(MouseEvent *mouseevent);

    void show_hardware_cursor();
    void hide_hardware_cursor();
} // namespace Mouse

#endif