#ifndef TWEEN_H
#define TWEEN_H

#pragma once

#include "drawobj.h"

namespace Tween
{
    void add_tween(DrawObject &drawObject, int duration, int x, int y, void (*cb)(void) = 0x0);
    void handle_tweens(double delta_time);
}; // namespace Tween

#endif