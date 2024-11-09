#ifndef ENGINE_RANDOM_H
#define ENGINE_RANDOM_H

#pragma once

namespace Engine
{
    namespace Random
    {
        void init();
        int integer_between(int min, int max);
        double double_between(double min, double max);
    }
}

#endif // ENGINE_RANDOM_H