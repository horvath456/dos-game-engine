#include "adlib.h"

#include <dos.h>
#include <conio.h>
#include <iostream.h>
#include <fstream>
#include <stdio>
#include <stdlib>
#include "timer.h"

#define ADDRESS_STATUS_PORT 0x388
#define DATA_PORT 0x389

namespace Adlib
{

void callback_handler();
void write_register(unsigned char num, unsigned char value);
unsigned char read_status_register();
void delay_using_read(unsigned int iterations);
unsigned char calc_register_offset(unsigned char channel);

unsigned char far *music_buffer = NULL;
unsigned char far *sfx_buffer = NULL;
bool is_music_playing = false;
bool is_sfx_playing = false;
unsigned int music_pos = 0;
unsigned int sfx_pos = 0;
unsigned int music_buffer_length = 0;
unsigned int sfx_buffer_length = 0;
unsigned long long music_game_time = 0;
unsigned long long sfx_game_time = 0;
unsigned int music_rate = 0;
unsigned int sfx_rate = 0;
int adlib_is_present = -1;

void play_music(const char *filename, unsigned int rate)
{
    if (is_music_playing)
        return;

    std::fstream infile(filename, std::ios::in | std::ios::binary);

    infile.seekg(0, std::ios::end);
    unsigned int size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    unsigned char buf[2];
    infile.get(buf[0]);
    infile.get(buf[1]);
    unsigned int size_in_imf_file = buf[0] | (buf[1]) << 8;

    if (size_in_imf_file > 0)
    {
        music_buffer_length = size_in_imf_file;
    }
    else
    {
        music_buffer_length = size - 2;
    }

    music_buffer = new unsigned char[music_buffer_length];

    for (unsigned int i = 0; i < music_buffer_length; i++)
    {
        unsigned char c;
        infile.get(c);
        music_buffer[i] = c;
    }

    is_music_playing = true;
    music_rate = rate;
    music_pos = 0;
    music_game_time = Timer::get_time();

    Timer::add_callback(&callback_handler);
}

void play_sfx(const char *filename, unsigned int rate)
{
    if (is_sfx_playing)
        return;

    std::fstream infile(filename, std::ios::in | std::ios::binary);

    infile.seekg(0, std::ios::end);
    unsigned int size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    unsigned char buf[2];
    infile.get(buf[0]);
    infile.get(buf[1]);
    unsigned int size_in_imf_file = buf[0] | (buf[1]) << 8;

    if (size_in_imf_file > 0)
    {
        sfx_buffer_length = size_in_imf_file;
    }
    else
    {
        sfx_buffer_length = size - 2;
    }

    sfx_buffer = new unsigned char[sfx_buffer_length];

    for (unsigned int i = 0; i < sfx_buffer_length; i++)
    {
        unsigned char c;
        infile.get(c);
        sfx_buffer[i] = c;
    }

    is_sfx_playing = true;
    sfx_rate = rate;
    sfx_pos = 0;
    sfx_game_time = Timer::get_time();

    Timer::add_callback(&callback_handler);
}

void stop_music()
{
    if (is_music_playing)
    {
        stop(2);
        stop(3);
        stop(4);
        stop(5);
        stop(6);
        stop(7);
        stop(8);
        stop(9);
        is_music_playing = false;
        delete[] music_buffer;
        music_buffer = NULL;
    }
}

void stop_sfx()
{
    if (is_sfx_playing)
    {
        is_sfx_playing = false;
        delete[] sfx_buffer;
        sfx_buffer = NULL;
    }
}

void callback_handler()
{
    if (is_music_playing)
    {
        unsigned int wait_ticks = music_buffer[music_pos + 2] | (music_buffer[music_pos + 3]) << 8;
        unsigned int wait_time = wait_ticks * (Timer::get_frequency() / music_rate);
        if (Timer::get_time() - music_game_time >= wait_time)
        {
            write_register(music_buffer[music_pos], music_buffer[music_pos + 1]);

            music_pos += 4;
            if (music_pos >= music_buffer_length)
                music_pos = 0;

            music_game_time = Timer::get_time();
        }
    }
    if (is_sfx_playing)
    {
        unsigned int wait_ticks = sfx_buffer[sfx_pos + 2] | (sfx_buffer[sfx_pos + 3]) << 8;
        unsigned int wait_time = wait_ticks * (Timer::get_frequency() / sfx_rate);
        if (Timer::get_time() - sfx_game_time >= wait_time)
        {
            write_register(sfx_buffer[sfx_pos], sfx_buffer[sfx_pos + 1]);

            sfx_pos += 4;
            if (sfx_pos >= sfx_buffer_length)
            {
                stop_sfx();
            }

            sfx_game_time = Timer::get_time();
        }
    }
}

void play(unsigned char channel, unsigned int freq, unsigned char octave, unsigned char waveform)
{
    unsigned char register_offset = calc_register_offset(channel);
    write_register(0x20 + register_offset, 0x01);
    write_register(0x40 + register_offset, 0x10);
    write_register(0x60 + register_offset, 0xF0);
    write_register(0x80 + register_offset, 0x77);
    write_register(0xE0 + register_offset, waveform);
    write_register(0xA0 + channel - 1, (unsigned char)(freq & 0xff));
    write_register(0x23 + register_offset, 0x01);
    write_register(0x43 + register_offset, 0x00);
    write_register(0x63 + register_offset, 0xF0);
    write_register(0x83 + register_offset, 0x77);
    write_register(0xE3 + register_offset, waveform);
    write_register(0xB0 + channel - 1, ((freq & 0x300) >> 8) | ((octave & 0x7) << 2) | 0x20);
}

void stop(unsigned char channel)
{
    delay_using_read(35);
    write_register(0xB0 + calc_register_offset(channel), 0x11);
}

unsigned char calc_register_offset(unsigned char channel)
{
    if (channel <= 3)
        return channel - 1;
    if (channel <= 6)
        return channel + 4;
    return channel + 9;
}

bool check_if_adlib_present()
{
    if (adlib_is_present == -1)
    {
        write_register(4, 0x60);
        write_register(4, 0x80);
        unsigned char status1 = read_status_register();
        write_register(2, 0xFF);
        write_register(4, 0x21);
        delay_using_read(800);
        unsigned char status2 = read_status_register();
        write_register(4, 0x60);
        write_register(4, 0x80);
        adlib_is_present = (((status1 & 0xE0) == 0x0) && ((status2 & 0xE0) == 0x0C0));
    }
    return adlib_is_present;
}

void init_adlib()
{
    reset_card();
    write_register(0x1, 32);
    write_register(0xB0, 0x11);
    write_register(0x4, 0x60);
    write_register(0x4, 0x80);
}

void write_register(unsigned char num, unsigned char value)
{
    outp(ADDRESS_STATUS_PORT, num);
    delay_using_read(6);
    outp(DATA_PORT, value);
    delay_using_read(35);
}

unsigned char read_status_register()
{
    return inp(ADDRESS_STATUS_PORT);
}

void delay_using_read(unsigned int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        read_status_register();
    }
}

void reset_card()
{
    if (is_music_playing)
        stop_music();
    if (is_sfx_playing)
        stop_sfx();

    for (unsigned char i = 0x01; i <= 0xF5; i++)
    {
        write_register(i, 0x0);
    }

    write_register(0x20, 0x41);
    write_register(0x40, 0x10);
    write_register(0x60, 0xF0);
    write_register(0x80, 0x77);
    write_register(0x23, 0x41);
    write_register(0x43, 0x0);
    write_register(0x63, 0xF0);
    write_register(0x83, 0x77);
    write_register(0xBD, 0x10);
}

} // namespace Adlib