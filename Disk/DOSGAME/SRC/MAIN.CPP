#include <stdio>
#include <conio.h>
#include <dos.h>
#include <string>
#include <iostream>

#include "screen.h"
#include "drawobj.h"
#include "palette.h"
#include "timer.h"
#include "adlib.h"
#include "keyboard.h"
#include "speaker.h"
#include "text.h"
#include "font.h"
#include "mouse.h"
#include "joystick.h"
#include "draw.h"
#include "sprsheet.h"
#include "sprite.h"
#include "tween.h"
#include "random.h"

int main()
{
	Screen screen = Screen();

	Keyboard::init_keyboard();
	Timer::init_timer(7000);

	bool mouse_installed = Mouse::init_mouse();
	bool joystick_installed = Joystick::check_if_joystick_is_installed();

	if (Adlib::check_if_adlib_present())
	{
		Adlib::init_adlib();
		Adlib::play_music("DATA\\MUSIC.IMF", 700);
	}

	Palette palette = Palette();
	palette.load_palette_file("DATA\\PALETTE.DAT");

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
	gems_sheet.~Spritesheet(); // free memory since its scarce

	unsigned char board[10][10] = {
		{0, 0, 1, 2, 3, 1, 1, 2, 3, 0},
		{2, 1, 3, 1, 1, 0, 1, 2, 1, 0},
		{0, 0, 1, 2, 3, 1, 1, 2, 3, 0},
		{2, 1, 3, 1, 1, 0, 1, 2, 1, 0},
		{0, 0, 1, 2, 3, 1, 1, 2, 3, 0},
		{2, 1, 3, 1, 1, 0, 1, 2, 1, 0},
		{0, 0, 1, 2, 3, 1, 1, 2, 3, 0},
		{2, 1, 3, 1, 1, 0, 1, 2, 1, 0},
		{0, 0, 1, 2, 3, 1, 1, 2, 3, 0},
		{2, 1, 3, 1, 1, 0, 1, 2, 1, 0}};

	unsigned int highlighted[] = {0, 0};

	unsigned int soundeffect1[] = {2000, 50, 3000, 15, 1800, 25, 0, 0};
	unsigned int soundeffect2[] = {900, 100, 700, 80, 1300, 50, 0, 0};

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
				Draw::draw_sprite(screen, gems_sprites[board[i][j]], 80 + j * 16, 30 + i * 16, true);
				if (highlighted[0] == i && highlighted[1] == j)
				{
					Draw::hline(screen, 80 + j * 16, 30 + i * 16, 6, 3);
					Draw::hline(screen, 80 + j * 16, 30 + i * 16 + 1, 6, 3);
					Draw::vline(screen, 80 + j * 16, 30 + i * 16, 6, 3);
					Draw::vline(screen, 80 + j * 16 + 1, 30 + i * 16, 6, 3);
					Draw::hline(screen, 80 + j * 16 + 10, 30 + i * 16 + 14, 6, 3);
					Draw::hline(screen, 80 + j * 16 + 10, 30 + i * 16 + 15, 6, 3);
					Draw::vline(screen, 80 + j * 16 + 14, 30 + i * 16 + 10, 6, 3);
					Draw::vline(screen, 80 + j * 16 + 15, 30 + i * 16 + 10, 6, 3);
				}
			}
		}

		if (mouse_installed)
		{
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
						board[i][j] = Random::integer_between(0, 7);
						highlighted[0] = i;
						highlighted[1] = j;
						Speaker::play_sfx(soundeffect2, sizeof(soundeffect1) / sizeof(soundeffect1[0]));
					}
					else
					{
						Speaker::play_sfx(soundeffect1, sizeof(soundeffect2) / sizeof(soundeffect2[0]));
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
