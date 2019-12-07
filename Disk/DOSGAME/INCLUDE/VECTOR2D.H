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
    Vector2D()
    {
        x = 0;
        y = 0;
    }

    Vector2D operator+(Vector2D const &obj)
    {
        Vector2D vec;
        vec.x = x + obj.x;
        vec.y = y + obj.y;
        return vec;
    }

    Vector2D operator-(Vector2D const &obj)
    {
        Vector2D vec;
        vec.x = x - obj.x;
        vec.y = y - obj.y;
        return vec;
    }
    Vector2D operator*(Vector2D const &obj)
    {
        Vector2D vec;
        vec.x = x * obj.x;
        vec.y = y * obj.y;
        return vec;
    }

    Vector2D operator/(Vector2D const &obj)
    {
        Vector2D vec;
        vec.x = x / obj.x;
        vec.y = y / obj.y;
        return vec;
    }

    Vector2D operator+(double v)
    {
        Vector2D vec;
        vec.x = x + v;
        vec.y = y + v;
        return vec;
    }

    Vector2D operator-(double v)
    {
        Vector2D vec;
        vec.x = x - v;
        vec.y = y - v;
        return vec;
    }

    Vector2D operator*(double v)
    {
        Vector2D vec;
        vec.x = x * v;
        vec.y = y * v;
        return vec;
    }

    Vector2D operator/(double v)
    {
        Vector2D vec;
        vec.x = x / v;
        vec.y = y / v;
        return vec;
    }

    double x;
    double y;
};

#endif