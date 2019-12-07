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

void main()
{
	Screen screen = Screen();
	Timer timer = Timer(100);
	Keyboard keyboard = Keyboard();

	if (Adlib::check_if_adlib_present())
	{
		cout << "Adlib detected!!" << endl;
		Adlib adlib = Adlib();
		adlib.play(1, 300, 3, 1);
		sleep(1);
		adlib.stop(1);
	}

	Palette::load_palette("DATA\\PALETTE.DAT");

	DrawObject background = DrawObject("IMAGES\\BACKGRND.SPR");
	DrawObject grass = DrawObject("IMAGES\\GRASS.SPR");

	Player player = Player("IMAGES\\PLAYER.SPR");

	while (1 == 1)
	{
		if(keyboard.check_if_pressed(1)) break;

		if(keyboard.check_if_pressed(75)) player.move(-1, 0);
		if(keyboard.check_if_pressed(77)) player.move(1, 0);

		screen.draw_background(background.get_buffer());
		for (int i = 0; i < 20; i++)
		{
			grass.draw(screen, i * 16, 200 - 16);
		}
		player.draw(screen);

		screen.draw_screen_buffer();
	}
}