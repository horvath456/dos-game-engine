#include "player.h"

#include "screen.h"
#include "drawobj.h"

Player::Player(const char *filename)
{
    drawobj = DrawObject(filename);
}

void Player::draw(Screen &screen)
{
    drawobj.draw(screen, get_pos_x(), get_pos_y(), 1);
}