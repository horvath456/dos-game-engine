#ifndef ENGINE_TWEEN_H
#define ENGINE_TWEEN_H

#pragma once

#include "vec2d.h"

namespace Engine
{
    namespace Tween
    {
        void add_tween(Vec2D &vector, int duration, int x, int y, void (*cb)(void) = 0x0);
        void handle_tweens(double delta_time);
    };
};

#endif // ENGINE_TWEEN_H