#include "text.h"

#include "font.h"
#include "screen.h"
#include <cstdint>
#include <cstring>

std::uint32_t mask[16] = {
    0x00000000,
    0xFF000000,
    0x00FF0000,
    0xFFFF0000,
    0x0000FF00,
    0xFF00FF00,
    0x00FFFF00,
    0xFFFFFF00,
    0x000000FF,
    0xFF0000FF,
    0x00FF00FF,
    0xFFFF00FF,
    0x0000FFFF,
    0xFF00FFFF,
    0x00FFFFFF,
    0xFFFFFFFF,
};

namespace Text
{
    void putchar(char c, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y)
    {
        unsigned char far *src = font.get_buffer(c);
        std::uint32_t fgcolor32 = color | ((std::uint32_t)color << 8) | ((std::uint32_t)color << 16) | ((std::uint32_t)color << 24);

        unsigned char max_cy = font.height;
        unsigned char max_cx = (font.width + 7) >> 3;

        for (unsigned char cy = 0; cy < max_cy; cy++)
        {
            for (unsigned char cx = 0; cx < max_cx; cx++)
            {
                std::uint32_t mask_low = mask[(*src) & 0x0F];
                std::uint32_t mask_high = mask[((*src++) & 0xF0) >> 4];
                std::uint32_t far *screenbuf32 = (std::uint32_t far *)screen.get_buffer(x + (cx << 3), y + cy);
                screenbuf32[1] = (screenbuf32[1] & ~mask_low) | (fgcolor32 & mask_low);
                screenbuf32[0] = (screenbuf32[0] & ~mask_high) | (fgcolor32 & mask_high);
            }
        }
    }

    void putchar_narrow(char c, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y)
    {
        unsigned char far *src = font.get_buffer(c);
        std::uint32_t fgcolor32 = color | ((std::uint32_t)color << 8) | ((std::uint32_t)color << 16) | ((std::uint32_t)color << 24);

        unsigned char max_cy = font.height;

        for (unsigned char cy = 0; cy < max_cy; cy++)
        {
            std::uint32_t mask_low = mask[(*src) & 0x0F];
            std::uint32_t mask_high = mask[((*src++) & 0xF0) >> 4];
            std::uint32_t far *screenbuf32 = (std::uint32_t far *)screen.get_buffer(x, y + cy);
            screenbuf32[1] = (screenbuf32[1] & ~mask_low) | (fgcolor32 & mask_low);
            screenbuf32[0] = (screenbuf32[0] & ~mask_high) | (fgcolor32 & mask_high);
        }
    }

    void write_text(const char *str, Font &font, unsigned char color, Screen &screen, unsigned int x, unsigned int y)
    {
        char c = *str++;
        if (font.width == 8)
        {
            while (c != 0x0)
            {
                putchar_narrow(c, font, color, screen, x, y);
                x += 8;
                c = *str++;
            }
        }
        else
        {
            while (c != 0x0)
            {
                putchar(c, font, color, screen, x, y);
                x += font.width;
                c = *str++;
            }
        }
    }

    void write_text_center(const char *str, Font &font, unsigned char color, Screen &screen, unsigned int y, unsigned int center_x)
    {
        unsigned int calc_x = 160 - (font.width * std::strlen(str)) / 2;
        write_text(str, font, color, screen, calc_x, y);
    }
} // namespace Text