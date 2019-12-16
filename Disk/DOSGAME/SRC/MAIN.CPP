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
#include "player.h"
#include "keyboard.h"
#include "speaker.h"
#include "text.h"
#include "font.h"
#include "mouse.h"
#include "joystick.h"
#include "level.h"

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

	DrawObject background = DrawObject("IMAGES\\BACKGRND.SPR");
	DrawObject grass = DrawObject("IMAGES\\GRASS.SPR");
	DrawObject mouse = DrawObject("IMAGES\\MOUSE.SPR");

	Player player = Player("IMAGES\\PLAYER.ANM");

	Level level = Level("DATA\\LEVEL.LVL");
	level.add_tile(&grass);

	palette.blackout_palette();
	palette.fade_in();

	unsigned long long game_time = Timer::get_time();
	double delta_time = 0;

	while (true)
	{
		Screen::wait_retrace();

		delta_time = Timer::get_delta_time(game_time);
		game_time = Timer::get_time();

		if (Keyboard::check_if_pressed(1))
			break;

		screen.draw_background(background.get_buffer());

		player.update(delta_time * 3, level);
		level.render(screen, player);

		Text::write_text((Keyboard::check_if_pressed(75)) ? "left press" : "no press", font, 20, screen, 10, 50);
		Text::write_text((Keyboard::check_if_pressed(77)) ? "right press" : "no press", font, 20, screen, 10, 80);

		if (mouse_installed)
		{
			struct Mouse::MouseStatus mouse_status = Mouse::get_position_and_button_status();
			mouse.draw(screen, mouse_status.coors.x, mouse_status.coors.y, true);
			char str[25];
			sprintf(str, "FPS: %f\0", 1 / delta_time);
			Text::write_text(str, font, 20, screen, 10, 110);
		}

		screen.draw_screen_buffer();
	}

	Keyboard::deinit_keyboard();
	Timer::deinit_timer();
	if (Adlib::check_if_adlib_present())
		Adlib::reset_card();

	return 0;
}