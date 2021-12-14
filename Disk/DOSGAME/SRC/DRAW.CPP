#include "draw.h"

#include "screen.h"
#include <cstdint>

namespace Draw
{
    void putpixel(Screen &screen, unsigned int x, unsigned int y, unsigned char color)
    {
        screen.plot_pixel(x, y, color);
    }

    void hline(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned char color)
    {
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
        for (unsigned int i = 0; i < width; i++)
        {
            *screenbuf++ = color;
        }
    }

    void vline(Screen &screen, unsigned int x, unsigned int y, unsigned int height, unsigned char color)
    {
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
        for (unsigned int i = 0; i < height; i++)
        {
            *screenbuf = color;
            screenbuf += Screen::HPIXELS;
        }
    }

    void rect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color)
    {
        Draw::hline(screen, x, y, width, color);
        Draw::hline(screen, x, y + height, width, color);
        Draw::vline(screen, x, y, height, color);
        Draw::vline(screen, x + width, y, height, color);
    }

    void fillrect(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color)
    {
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
        for (unsigned int i = 0; i < height; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                *screenbuf++ = color;
            }
            screenbuf += Screen::HPIXELS - width;
        }
    }

    void rect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width,
                    unsigned int height, unsigned char color, unsigned char radius)
    {
        if (radius == 2)
        {
            Draw::hline(screen, x + 3, y, width - 5, color);
            Draw::hline(screen, x + 3, y + height, width - 5, color);
            Draw::vline(screen, x, y + 3, height - 5, color);
            Draw::vline(screen, x + width, y + 3, height - 5, color);
            Draw::putpixel(screen, x + 1, y + 2, color);
            Draw::putpixel(screen, x + 2, y + 1, color);
            Draw::putpixel(screen, x + width - 1, y + 2, color);
            Draw::putpixel(screen, x + width - 2, y + 1, color);
            Draw::putpixel(screen, x + 1, y + height - 2, color);
            Draw::putpixel(screen, x + 2, y + height - 1, color);
            Draw::putpixel(screen, x + width - 1, y + height - 2, color);
            Draw::putpixel(screen, x + width - 2, y + height - 1, color);
        }
        else if (radius == 1)
        {
            Draw::hline(screen, x + 1, y, width - 2, color);
            Draw::hline(screen, x + 1, y + height, width - 2, color);
            Draw::vline(screen, x, y + 1, height - 2, color);
            Draw::vline(screen, x + width, y + 1, height - 2, color);
        }
    }

    void fillrect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height,
                        unsigned char color, unsigned char outline_color, unsigned char radius)
    {
        if (radius == 2)
        {
            Draw::rect_round(screen, x, y, width, height, outline_color);
            Draw::fillrect(screen, x + 3, y + 1, width - 5, height - 1, color);
            Draw::fillrect(screen, x + 1, y + 3, 2, height - 5, color);
            Draw::fillrect(screen, x + width - 2, y + 3, 2, height - 5, color);
            Draw::putpixel(screen, x + 2, y + 2, color);
            Draw::putpixel(screen, x + width - 2, y + 2, color);
            Draw::putpixel(screen, x + 2, y + height - 2, color);
            Draw::putpixel(screen, x + width - 2, y + height - 2, color);
        }
        else if (radius == 1)
        {
            Draw::fillrect(screen, x + 1, y + 1, width - 2, height - 2, color);
            Draw::hline(screen, x + 1, y, width - 2, outline_color);
            Draw::hline(screen, x + 1, y + height, width - 2, outline_color);
            Draw::vline(screen, x, y + 1, height - 2, outline_color);
            Draw::vline(screen, x + width, y + 1, height - 2, outline_color);
        }
    }

    void dithering_pattern(Screen &screen, unsigned int x, unsigned int y, unsigned int width,
                           unsigned int height, unsigned char color, unsigned char density)
    {
        // ignore density for now, assume height>=2, width >=2
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
        for (unsigned int i = 0; i < height / 2; i++)
        {
            for (unsigned int j = 0; j < width; j++)
            {
                *screenbuf++ = color;
                j++;
                screenbuf++;
            }
            screenbuf += Screen::HPIXELS - width;
            for (unsigned int j = 0; j < width; j++)
            {
                j++;
                screenbuf++;
                *screenbuf++ = color;
            }
            screenbuf += Screen::HPIXELS - width;
        }
    }
} // namespace Draw