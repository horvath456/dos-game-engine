#include "adlib.h"

#include <dos.h>
#include <time.h>
#include <fstream.h>
#include <iostream.h>
#include "timer.h"

Adlib::Adlib()
{
    reset_card();
    init();
}

Adlib::~Adlib()
{
    reset_card();
}

void Adlib::play_music(const char *filename, Timer &t)
{
    fstream infile = fstream(filename, ios::in | ios::binary);

    unsigned char buf[4];

    infile.get(buf[0]);
    infile.get(buf[1]);

    unsigned int num_notes = (buf[0] << 8) + buf[1];

    buffer = new far unsigned char[(unsigned int)(num_notes * 7)];

    for (int i = 0; i < num_notes * 7; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }

    is_music_playing = TRUE;
    music_pos = 0;
    game_time = t.get_time();
    buffer_length = num_notes * 7;
    timer = &t;
    self = this;

    self->play(1, (buffer[music_pos + 1] << 8) + buffer[music_pos + 2], buffer[music_pos + 3], buffer[music_pos + 4]);

    timer->add_callback(&callback_handler);
}

void Adlib::stop_music()
{
    if (is_music_playing)
    {
        play_music_end();
    }
}

void Adlib::play_music_end()
{
    timer->remove_callback(&callback_handler);
    self->stop(1);
    is_music_playing = FALSE;
    delete[] buffer;
    buffer = NULL;
}

void Adlib::callback_handler()
{
    if (timer->get_delta_time(game_time) * 1000 >= (buffer[music_pos + 5] << 8) + buffer[music_pos + 6])
    {
        music_pos += 7;

        if (music_pos >= buffer_length)
        {
            play_music_end();
            return;
        }

        game_time = timer->get_time();

        self->play(1, (buffer[music_pos + 1] << 8) + buffer[music_pos + 2], buffer[music_pos + 3], buffer[music_pos + 4]);
    }
}

void Adlib::play(unsigned char channel, unsigned int freq, unsigned char octave, unsigned char waveform)
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

void Adlib::stop(unsigned char channel)
{
    delay_using_read(35);
    write_register(0xB0 + calc_register_offset(channel), 0x11);
}

unsigned char Adlib::calc_register_offset(unsigned char channel)
{
    if (channel <= 3)
        return channel - 1;
    if (channel <= 6)
        return channel + 4;
    return channel + 9;
}

int Adlib::check_if_adlib_present()
{
    if (is_present == -1)
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
        is_present = (((status1 & 0xE0) == 0x0) && ((status2 & 0xE0) == 0x0C0));
        return (((status1 & 0xE0) == 0x0) && ((status2 & 0xE0) == 0x0C0));
    }
    else
    {
        return is_present;
    }
}

void Adlib::init()
{
    write_register(0x1, 32);
    write_register(0xB0, 0x11);
    write_register(0x4, 0x60);
    write_register(0x4, 0x80);
}

void Adlib::write_register(unsigned char num, unsigned char value)
{
    outp(ADDRESS_STATUS_PORT, num);
    delay_using_read(6);
    outp(DATA_PORT, value);
    delay_using_read(35);
}

unsigned char Adlib::read_status_register()
{
    return inp(ADDRESS_STATUS_PORT);
}

void Adlib::delay_using_read(unsigned int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        read_status_register();
    }
}

void Adlib::reset_card()
{
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

unsigned char far *Adlib::buffer;
int Adlib::is_music_playing;
unsigned int Adlib::music_pos;
unsigned long long Adlib::game_time;
unsigned long int Adlib::buffer_length;
Timer *Adlib::timer;
Adlib *Adlib::self = NULL;

int Adlib::is_present = -1;