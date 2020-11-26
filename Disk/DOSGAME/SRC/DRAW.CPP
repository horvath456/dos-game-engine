#include "draw.h"

#include "screen.h"
#include <cstdint>

namespace Draw
{
    inline void putpixel(Screen &screen, unsigned int x, unsigned int y, unsigned char color)
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

    void rect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color, unsigned int radius)
    {
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
    }

    void fillrect_round(Screen &screen, unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char color, unsigned int radius)
    {
        std::uint8_t far *screenbuf = (std::uint8_t far *)screen.get_buffer(x, y);
    }
} // namespace Draw