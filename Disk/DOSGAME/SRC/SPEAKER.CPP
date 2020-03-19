#include "speaker.h"

#include "timer.h"
#include <dos.h>
#include <conio.h>
#include <iostream.h>
#include <fstream>
#include <stdio>
#include <stdlib>
#include <dos.h>
#include <string.h>

namespace Speaker
{

void callback_handler();
void play_music_end();

unsigned char far *buffer = NULL;
bool is_music_playing = false;
bool loop = false;

bool play_string_music = false;
unsigned int octave = 2;
unsigned int duration = 4;
unsigned int curr_wait = 0;
unsigned int tempo = 120;

unsigned int music_pos = 0;
unsigned long long game_time = 0;
unsigned long int buffer_length = 0;

void Speaker::play_music_string(const char *play_string, bool loop_music)
{
    stop_music();

    buffer = new unsigned char[strlen(play_string)];

    while (*play_string != 0)
        *buffer++ = *play_string++;

    is_music_playing = true;
    music_pos = 0;
    game_time = Timer::get_time();
    buffer_length = strlen(play_string);
    play_string_music = true;
    loop = loop_music;

    Timer::add_callback(&callback_handler);
}

void Speaker::play_music(const char *filename, bool loop_music)
{
    stop_music();

    std::fstream infile(filename, std::ios::in | std::ios::binary);

    unsigned char buf[2];
    infile.get(buf[0]);
    infile.get(buf[1]);
    unsigned int size = (buf[1] | (buf[0]) << 8) * 4;

    buffer = new unsigned char[size];

    for (unsigned int i = 0; i < size; i++)
    {
        unsigned char c;
        infile.get(c);
        buffer[i] = c;
    }

    is_music_playing = true;
    music_pos = 0;
    game_time = Timer::get_time();
    buffer_length = size;
    loop = loop_music;
    play_string_music = false;

    Timer::add_callback(&callback_handler);
}

void Speaker::stop_music()
{
    if (is_music_playing)
    {
        play_music_end();
    }
}

void Speaker::play_music_end()
{
    Timer::remove_callback(&callback_handler);
    stop();
    is_music_playing = false;
    delete[] buffer;
    buffer = NULL;
}

void Speaker::callback_handler()
{
    unsigned int wait_time = (unsigned int)buffer[music_pos + 3] | (unsigned int)((unsigned int)buffer[music_pos + 2] << 8);
    unsigned int note = (unsigned int)buffer[music_pos + 1] | (unsigned int)((unsigned int)buffer[music_pos] << 8);
    if (Timer::get_delta_time(game_time) * 1000 >= wait_time)
    {
        game_time = Timer::get_time();
        if (note > 0)
            play(note);
        else
            stop();
        music_pos += 4;
    }

    if (music_pos >= buffer_length && !loop)
        play_music_end();
    if (music_pos >= buffer_length && loop)
        music_pos = 0;
}

//Play sound using built in speaker
void Speaker::play(unsigned long int freq)
{
    //Set the PIT to the desired frequency
    unsigned long int div = 1193180 / freq;
    outp(0x43, 0xb6);
    outp(0x42, (unsigned char)(div));
    outp(0x42, (unsigned char)(div >> 8));

    //And play the sound using the PC speaker
    unsigned char tmp = inp(0x61);
    if (tmp != (tmp | 3))
    {
        outp(0x61, tmp | 3);
    }
}

//make it shutup
void Speaker::stop()
{
    unsigned char tmp = inp(0x61) & 0xFC;
    outp(0x61, tmp);
}
} // namespace Speaker