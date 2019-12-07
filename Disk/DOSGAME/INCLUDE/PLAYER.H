#ifndef PLAYER_H
#define PLAYER_H

#pragma once

#include "screen.h"
#include "drawobj.h"
#include "movable.h"

class Player : public Movable
{
public:
    Player(const char *filename);

    void draw(Screen &screen);

private:
    DrawObject drawobj;
};

#endif