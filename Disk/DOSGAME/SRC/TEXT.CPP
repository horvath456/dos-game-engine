#include "text.h"

#include "font.h"
#include "screen.h"
#include <cstdint>

std::uint32_t mask[16] = {
    0x00000000, // for 0b0000
    0x000000FF, // for 0b0001
    0x0000FF00, // for 0b0010
    0x0000FFFF, // for 0b0011
    0x00FF0000, // for 0b0100
    0x00FF00FF, // for 0b0101
    0x00FFFF00, // for 0b0110
    0x00FFFFFF, // for 0b0111
    0xFF000000, // for 0b1000
    0xFF0000FF, // for 0b1001
    0xFF00FF00, // for 0b1010
    0xFF00FFFF, // for 0b1011
    0xFFFF0000, // for 0b1100
    0xFFFF00FF, // for 0b1101
    0xFFFFFF00, // for 0b1110
    0xFFFFFFFF  // for 0b1111
};

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
    std::uint32_t fgcolor32 = (std::uint32_t)color | ((std::uint32_t)color << 8) | ((std::uint32_t)color << 16) | ((std::uint32_t)color << 24);

    for (unsigned int cy = 0; cy < font.height; cy++)
    {
        for (unsigned int cx = 0; cx < (font.width + 7) / 8; cx++)
        {
            std::uint32_t mask_low = mask[(*src) & 0x0F];
            std::uint32_t mask_high = mask[((*src++) & 0xF0) >> 4];
            std::uint32_t far *screenbuf32 = (std::uint32_t far *)screen.get_buffer(x + cx * 8, y + cy);
            screenbuf32[0] = (screenbuf32[0] & ~mask_low) | (fgcolor32 & mask_low);
            screenbuf32[1] = (screenbuf32[1] & ~mask_high) | (fgcolor32 & mask_high);
        }
    }
}
} // namespace Text