#include <iostream.h>
#include <conio.h>
#include <dos.h>

#include "screen.h"
#include "drawobj.h"
#include "palette.h"
#include "timer.h"
#include "adlib.h"

void main()
{
	Screen screen = Screen();

	Timer timer = Timer(100);

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

	screen.draw_background(background.get_buffer());
	for (int i = 0; i < 20; i++)
	{
		grass.draw(screen, i * 16, 200 - 16);
	}

	screen.draw_screen_buffer();

	sleep(1);
}