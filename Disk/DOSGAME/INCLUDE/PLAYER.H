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

private:
    long int x;
    long int y;
    DrawObject drawobj;
};

#endif