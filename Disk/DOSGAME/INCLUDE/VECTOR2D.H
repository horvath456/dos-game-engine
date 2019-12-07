#ifndef VECTOR2D_H
#define VECTOR2D_H

#pragma once

class Vector2D
{
public:
    Vector2D(double _x, double _y) : x{_x}, y{_y} {}

    double x;
    double y;
};

#endif