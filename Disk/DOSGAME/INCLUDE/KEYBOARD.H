#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 57
#define KEY_ESC 1

namespace Keyboard
{
void init_keyboard();
void deinit_keyboard();

bool check_if_pressed(unsigned char scancode);

unsigned char get_last_action();
void clear_last_action();

bool get_key_up(unsigned char scancode);
bool get_key_down(unsigned char scancode);
} // namespace Keyboard

#endif