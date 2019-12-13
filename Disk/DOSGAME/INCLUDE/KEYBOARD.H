#ifndef KEYBOARD_H
#define KEYBOARD_H

#pragma once

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