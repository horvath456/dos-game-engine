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
	screen.set_graphics_mode();

	Timer timer = Timer();

	if (Adlib::check_if_adlib_present())
	{
		cout << "Adlib detected!!" << endl;
		sleep(1);
		Adlib adlib = Adlib();
		adlib.play();
		sleep(1);
		adlib.stop();
	}

	Palette::load_palette("DATA\\PALETTE.DAT");

	DrawObject draw_obj = DrawObject("IMAGES\\BACKGRND.SPR");
	draw_obj.draw(screen, 0, 0);

	screen.draw_screen_buffer();

	sleep(1);
}