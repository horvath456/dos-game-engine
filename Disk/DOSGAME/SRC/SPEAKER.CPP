#include "speaker.h"

#include "timer.h"
#include <dos.h>
#include <fstream.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void Speaker::play_music(const char *play_string)
{
    unsigned int num_bytes = strlen(play_string);

    buffer = new far unsigned char[(unsigned int)(num_bytes)];

    unsigned int size = num_bytes;

    while (size-- != 0)
        *buffer++ = *play_string++;

    is_music_playing = TRUE;
    music_pos = 0;
    game_time = Timer::get_time();
    buffer_length = num_bytes;

    stop_music();
    stop();

    Timer::add_callback(&callback_handler, (void *)0x0);
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
    is_music_playing = FALSE;
    delete[] buffer;
    buffer = NULL;
}

void Speaker::callback_handler(void *_)
{
    if (Timer::get_delta_time(game_time) * 1000 >= (buffer[music_pos + 2] << 8) + buffer[music_pos + 3])
    {
        game_time = Timer::get_time();
        play((buffer[music_pos] << 8) + buffer[music_pos + 1]);
        music_pos += 4;
    }
    if (music_pos >= buffer_length)
        play_music_end();
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

unsigned char far *Speaker::buffer;
int Speaker::is_music_playing;
unsigned int Speaker::music_pos;
unsigned long long Speaker::game_time;
unsigned long int Speaker::buffer_length;