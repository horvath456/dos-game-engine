#include <stdio>
#include <conio.h>
#include <dos.h>

#include "screen.h"
#include "drawobj.h"
#include "palette.h"
#include "timer.h"
#include "adlib.h"
#include "player.h"
#include "keyboard.h"
#include "speaker.h"

#define DELTA_TIME_FACTOR 15

using namespace std;

int main()
{
	Screen screen = Screen();

	if (Adlib::check_if_adlib_present())
		Adlib::init_adlib();
	Keyboard::init_keyboard();
	Timer::init_timer(1000);

	Palette palette = Palette();
	palette.load_palette_file("DATA\\PALETTE.DAT");

	DrawObject background = DrawObject("IMAGES\\BACKGRND.SPR");
	DrawObject grass = DrawObject("IMAGES\\GRASS.SPR");

	Player player = Player("IMAGES\\PLAYER.SPR");

	palette.blackout_palette();
	palette.fade_in();

	unsigned long long game_time = Timer::get_time();
	double delta_time = 0;

	while (1 == 1)
	{
		if (Keyboard::check_if_pressed(1))
			break;

		if (Keyboard::check_if_pressed(75))
			player.target_velocity.x = -1;
		if (Keyboard::check_if_pressed(77))
			player.target_velocity.x = 1;

		if (Keyboard::get_key_down(57))
			player.target_velocity.y = -1;

		player.update_physics(delta_time);

		while (!Screen::is_in_vertical_retrace())
			;

		screen.draw_background(background.get_buffer());

		for (int i = 0; i < 20; i++)
		{
			grass.draw(screen, i * 16, 200 - 16);
		}
		player.draw(screen);

		screen.draw_screen_buffer();

		delta_time = Timer::get_delta_time(game_time) * DELTA_TIME_FACTOR;
		game_time = Timer::get_time();
	}

	Keyboard::deinit_keyboard();
	Timer::deinit_timer();
	if (Adlib::check_if_adlib_present())
		Adlib::reset_card();

	return 0;
}