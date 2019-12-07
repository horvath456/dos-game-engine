#include "player.h"
#include "screen.h"
#include "drawobj.h"

#define START_X 100
#define START_Y 100

Player::Player(const char *filename)
{
    drawobj = DrawObject(filename);
    x = START_X;
    y = START_Y;
}

void Player::draw(Screen &screen)
{
    drawobj.draw(screen, x, y);
}

void Player::move(int delta_x, int delta_y)
{
    x += delta_x;
    y += delta_y;
}