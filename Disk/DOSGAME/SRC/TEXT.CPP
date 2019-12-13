#include "text.h"

#include "font.h"
#include "screen.h"

namespace Text
{
void write_text(const char *str, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y)
{
    char c = *str++;
    while (c != 0x0)
    {
        putchar(c, font, color, screen, x, y);
        x += font.width;
        c = *str++;
    }
}

void putchar(char c, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y)
{
    unsigned char far *src = font.get_buffer(c);
    int mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};

    for (int cy = 0; cy < font.height; cy++)
    {
        for (int cx = 0; cx < font.width; cx++)
        {
            if (src[cy] & mask[cx])
                screen.plot_pixel(x + cx, y + cy, color);
        }
    }
}
} // namespace Text