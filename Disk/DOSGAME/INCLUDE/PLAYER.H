#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "screen.h"
#include "drawobj.h"

class Player
{
public:
    Player(const char* filename);

    void draw(Screen &screen);

    void move(int delta_x, int delta_y);

private:
    long int x;
    long int y;
    long int vx;
    long int vy;
    DrawObject drawobj;
};

#endif