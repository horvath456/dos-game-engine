#ifndef VECTOR2D_H
#define VECTOR2D_H

#pragma once

class Vector2D
{
public:
    Vector2D(double _x, double _y)
    {
        x = _x;
        y = _y;
    }
    Vector2D() {
        x=0;
        y=0;
    }

    double x;
    double y;
};

#endif