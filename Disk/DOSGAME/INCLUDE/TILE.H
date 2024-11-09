#ifndef TILE_H
#define TILE_H

#pragma once

#include "vec2d.h"

class Tile
{
public:
    unsigned char type;
    Engine::Vec2D position;
};

#endif // TILE_H