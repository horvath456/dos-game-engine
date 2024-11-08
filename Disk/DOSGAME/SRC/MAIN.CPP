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

	unsigned int soundeffect1[] = {2000, 50, 3000, 15, 1800, 25, 0, 0};

	Sprite background = Sprite("IMAGES\\BACKGRND.SPR");
	DrawObject EB = DrawObject("IMAGES\\EB.SPR", 60, 120);
	DrawObject mouse = DrawObject("IMAGES\\MOUSE.SPR", 0, 0, true);

	palette.blackout_palette();
	palette.fade_in();

	unsigned long long game_time = Timer::get_time();
	double delta_time = 0;

	bool click_event = false;
	unsigned int lastmousedownx = 0;
	unsigned int lastmousedowny = 0;
	unsigned int lastmouseupx = 0;
	unsigned int lastmouseupy = 0;

	Mouse::MouseStatus mousestatus = Mouse::get_current_status();

	while (true)
	{
		Screen::wait_retrace();

		delta_time = Timer::get_delta_time(game_time);
		game_time = Timer::get_time();

		if (Keyboard::check_if_pressed(1))
			break;

		screen.draw_background(background.get_buffer());

		EB.draw(screen);

		char str[25];
		sprintf(str, "FPS: %f\0", 1 / delta_time);
		Text::write_text(str, font, 20, screen, 10, 0);

		if (mouse_installed)
		{
			mousestatus = Mouse::get_current_status();
			mouse.set_position(mousestatus.coors.x, mousestatus.coors.y);
			mouse.draw(screen);
		}

		Mouse::MouseEvent mouseevent;
		if (Mouse::get_mouse_click_event(&mouseevent))
		{
			click_event = true;
			if (mouseevent.type == Mouse::left_mouse_down)
			{
				lastmousedownx = mouseevent.coors.x;
				lastmousedowny = mouseevent.coors.y;
				Speaker::play_sfx(soundeffect1, sizeof(soundeffect1) / sizeof(soundeffect1[0]));
				Tween::add_tween(EB, 500, lastmousedownx, lastmousedowny, NULL);
			}
			else if (mouseevent.type == Mouse::left_mouse_up)
			{
				lastmouseupx = mouseevent.coors.x;
				lastmouseupy = mouseevent.coors.y;
			}
		}

		screen.draw_screen_buffer();

		Tween::handle_tweens(delta_time);
	}

	if (Adlib::check_if_adlib_present())
		Adlib::reset_card();

	Keyboard::deinit_keyboard();
	Timer::deinit_timer();

	palette.blackout_palette();
	Screen::wait_retrace();

	return 0;
}
