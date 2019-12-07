#include <iostream.h>
#include <conio.h>
#include <dos.h>

#include "screen.h"
#include "drawobj.h"
#include "palette.h"
#include "timer.h"

void main()
{
	Screen screen = Screen();
	screen.set_graphics_mode();

	Palette::load_palette("DATA\\PALETTE.DAT");

	Timer timer = Timer(5);

	DrawObject draw_obj = DrawObject("IMAGES\\BACKGRND.SPR");
	draw_obj.draw(screen, 0, 0);

	screen.draw_screen_buffer();

	sleep(1);
}