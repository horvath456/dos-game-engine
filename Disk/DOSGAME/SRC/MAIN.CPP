#include <stdio>
#include <conio.h>
#include <dos.h>
#include <string>
#include <iostream>

#include "engine.h"

#include "colors.h"
#include "tile.h"

using namespace Engine;

Screen screen = Screen();
bool mouse_installed = Mouse::init_mouse();

Font font = Font("DATA\\FONT.PSF");

Sprite background = Sprite("IMAGES\\BACKGRND.SPR");
DrawObject mouse = DrawObject("IMAGES\\MOUSE.SPR", 0, 0, true);

Spritesheet gems_sheet = Spritesheet("IMAGES\\GEMS.SPR");
Sprite gems_sprites[8] = {
	Sprite(gems_sheet, 0, 0, 16, 16),
	Sprite(gems_sheet, 16, 0, 16, 16),
	Sprite(gems_sheet, 32, 0, 16, 16),
	Sprite(gems_sheet, 48, 0, 16, 16),
	Sprite(gems_sheet, 64, 0, 16, 16),
	Sprite(gems_sheet, 80, 0, 16, 16),
	Sprite(gems_sheet, 96, 0, 16, 16),
	Sprite(gems_sheet, 112, 0, 16, 16)};

Tile board[10][10] = {0};

bool highlighted_tile = false;
unsigned int highlighted[] = {0, 0};

unsigned int soundeffect1[] = {2000, 50, 3000, 15, 1800, 25, 0, 0};
unsigned int soundeffect2[] = {900, 100, 700, 80, 1300, 50, 0, 0};

void handle_board_click(unsigned int i, unsigned int j)
{
	board[i][j].type = Random::integer_between(0, 7);
	highlighted_tile = true;
	highlighted[0] = i;
	highlighted[1] = j;
	Speaker::play_sfx(soundeffect2, sizeof(soundeffect1) / sizeof(soundeffect1[0]));
}

int main()
{
	Keyboard::init_keyboard();
	Timer::init_timer(7000);

	if (Adlib::check_if_adlib_present())
	{

		// Adlib::init_adlib();
		//  Adlib::play_music("DATA\\MUSIC.IMF", 700);
	}

	Palette palette = Palette();
	palette.load_palette_file("DATA\\PALETTE.DAT");

	palette.blackout_palette();
	palette.fade_in();

	unsigned long long game_time = Timer::get_time();
	double delta_time = 0;

	Mouse::MouseStatus mousestatus = Mouse::get_current_status();

	while (true)
	{
		Screen::wait_retrace();

		delta_time = Timer::get_delta_time(game_time);
		game_time = Timer::get_time();

		if (Keyboard::check_if_pressed(1))
			break;

		screen.draw_background(background.get_buffer());

		for (unsigned int i = 0; i < 10; i++)
		{
			for (unsigned int j = 0; j < 10; j++)
			{
				Draw::draw_sprite(screen, gems_sprites[board[i][j].type], 80 + j * 16, 30 + i * 16, true);
				if (highlighted[0] == i && highlighted[1] == j)
				{
					Draw::hline(screen, 80 + j * 16, 30 + i * 16, 6, Colors::PINK);
					Draw::hline(screen, 80 + j * 16, 30 + i * 16 + 1, 6, Colors::PINK);
					Draw::vline(screen, 80 + j * 16, 30 + i * 16, 6, Colors::PINK);
					Draw::vline(screen, 80 + j * 16 + 1, 30 + i * 16, 6, Colors::PINK);
					Draw::hline(screen, 80 + j * 16 + 10, 30 + i * 16 + 14, 6, Colors::PINK);
					Draw::hline(screen, 80 + j * 16 + 10, 30 + i * 16 + 15, 6, Colors::PINK);
					Draw::vline(screen, 80 + j * 16 + 14, 30 + i * 16 + 10, 6, Colors::PINK);
					Draw::vline(screen, 80 + j * 16 + 15, 30 + i * 16 + 10, 6, Colors::PINK);
				}
			}
		}

		if (mouse_installed)
		{
			Draw::hline(screen, 10, 10, 100, Colors::BLUE_DARK);
			mousestatus = Mouse::get_current_status();
			mouse.set_position(mousestatus.coors.x, mousestatus.coors.y);
			mouse.draw(screen);

			Mouse::MouseEvent mouseevent;
			if (Mouse::get_mouse_click_event(&mouseevent))
			{
				if (mouseevent.type == Mouse::left_mouse_up)
				{
					if (mouseevent.coors.x >= 80 && mouseevent.coors.x < 240 && mouseevent.coors.y >= 30 && mouseevent.coors.y < 190)
					{
						unsigned int i = (mouseevent.coors.y - 30) / 16;
						unsigned int j = (mouseevent.coors.x - 80) / 16;
						handle_board_click(i, j);
					}
				}
			}
		}

		screen.draw_screen_buffer();

		Tween::handle_tweens(delta_time);
	}

	if (Adlib::check_if_adlib_present())
		Adlib::reset_card();

	Keyboard::deinit_keyboard();
	Timer::deinit_timer();

	Screen::wait_retrace();

	return 0;
}
