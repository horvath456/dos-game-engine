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
		//Adlib::play_music("DATA\\MUSIC.IMF", 700);
	}

	Palette palette = Palette();
	palette.load_palette_file("DATA\\PALETTE.DAT");

	Font font = Font("DATA\\FONT.PSF");

	DrawObject background = DrawObject("IMAGES\\BACKGRND.SPR");
	DrawObject EA = DrawObject("IMAGES\\EA.SPR");
	DrawObject EB = DrawObject("IMAGES\\EB.SPR");
	DrawObject HK = DrawObject("IMAGES\\HK.SPR");
	DrawObject HZ = DrawObject("IMAGES\\HZ.SPR");
	DrawObject mouse = DrawObject("IMAGES\\MOUSE.SPR");

	palette.blackout_palette();
	palette.fade_in();

	unsigned long long game_time = Timer::get_time();
	double delta_time = 0;

	Speaker::play_music("DATA\\EFFECT.SFX");

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

		Text::write_text_center((Keyboard::check_if_pressed(75)) ? "left press" : "no press", font, 20, screen, 50);
		Text::write_text((Keyboard::check_if_pressed(77)) ? "right press" : "no press", font, 20, screen, 10, 80);

		EA.draw(screen, 10, 120, false);
		EB.draw(screen, 60, 120, false);
		HK.draw(screen, 110, 120, false);
		HZ.draw(screen, 160, 120, false);

		char str[25];
		sprintf(str, "FPS: %f\0", 1 / delta_time);
		Text::write_text(str, font, 20, screen, 10, 110);

		Draw::rect(screen, 10, 100, 50, 20, 20);
		Draw::fillrect(screen, 10, 130, 50, 20, 20);
		Draw::rect_round(screen, 100, 100, 50, 20, 20);
		Draw::fillrect_round(screen, 100, 130, 50, 20, 20, 0x3);

		if (mouse_installed)
		{
			mousestatus = Mouse::get_current_status();
			mouse.draw(screen, mousestatus.coors.x, mousestatus.coors.y, true);
		}

		Mouse::MouseEvent mouseevent;
		if (Mouse::get_mouse_click_event(&mouseevent))
		{
			click_event = true;
			if (mouseevent.type == Mouse::left_mouse_down)
			{
				lastmousedownx = mouseevent.coors.x;
				lastmousedowny = mouseevent.coors.y;
			}
			else if (mouseevent.type == Mouse::left_mouse_up)
			{
				lastmouseupx = mouseevent.coors.x;
				lastmouseupy = mouseevent.coors.y;
			}
		}

		if (click_event)
		{
			Text::write_text("CLICK", font, 20, screen, 80, 180);
			char debug[50] = {""};
			sprintf(debug, "%d %d  %d %d  C: %d %d", lastmousedownx, lastmousedowny, lastmouseupx, lastmouseupy, mousestatus.coors.x, mousestatus.coors.y);
			Text::write_text_center(debug, font, 20, screen, 20);
		}

		screen.draw_screen_buffer();
	}

	Keyboard::deinit_keyboard();
	Timer::deinit_timer();
	if (Adlib::check_if_adlib_present())
		Adlib::reset_card();

	return 0;
}
