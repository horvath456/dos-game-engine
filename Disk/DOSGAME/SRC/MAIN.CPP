#include <iostream.h>
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

void main()
{
	Screen screen = Screen();
	Timer timer = Timer(240);
	Keyboard keyboard = Keyboard();

	if (Adlib::check_if_adlib_present())
	{
		cout << "Adlib detected!!" << endl;
		Adlib adlib = Adlib();
		adlib.play(1, 300, 3, 1);
		sleep(1);
		adlib.stop(1);
	} else {
		cout << "Speaker detected!!" << endl;
		Speaker::play(300);
		sleep(1);
		Speaker::stop();
	}

	Palette::load_palette("DATA\\PALETTE.DAT");

	DrawObject background = DrawObject("IMAGES\\BACKGRND.SPR");
	DrawObject grass = DrawObject("IMAGES\\GRASS.SPR");

	Player player = Player("IMAGES\\PLAYER.SPR");

	unsigned long long game_time = timer.get_time();
	double delta_time = 0;

	while (1 == 1)
	{
		unsigned char last_keyboard_action = keyboard.get_last_action();

		if (keyboard.check_if_pressed(1))
			break;

		if (keyboard.check_if_pressed(75))
			player.target_velocity.x = -1;
		if (keyboard.check_if_pressed(77))
			player.target_velocity.x = 1;

		if (last_keyboard_action == 57)
			player.target_velocity.y = -1;

		player.update_physics(delta_time);

		screen.draw_background(background.get_buffer());
		for (int i = 0; i < 20; i++)
		{
			grass.draw(screen, i * 16, 200 - 16);
		}
		player.draw(screen);

		screen.draw_screen_buffer();

		delta_time = timer.get_delta_time(game_time) * 10;
		game_time = timer.get_time();
	}
}