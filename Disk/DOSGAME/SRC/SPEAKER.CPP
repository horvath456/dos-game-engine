#include "speaker.h"

#include "timer.h"
#include <dos.h>
#include <fstream.h>
#include <string.h>

namespace Speaker
{

void callback_handler();
void play_music_end();

unsigned char far *buffer = NULL;
bool is_music_playing = false;
bool loop = false;
unsigned int music_pos = 0;
unsigned long long game_time = 0;
unsigned long int buffer_length = 0;

void Speaker::play_music(const char *play_string, bool loop_music)
{
    unsigned int num_bytes = strlen(play_string);

    buffer = new far unsigned char[(unsigned int)(num_bytes)];

    unsigned int size = num_bytes;

    while (size-- != 0)
        *buffer++ = *play_string++;

    is_music_playing = true;
    music_pos = 0;
    game_time = Timer::get_time();
    buffer_length = num_bytes;
    loop = loop_music;

    stop_music();
    stop();

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
    if (Timer::get_delta_time(game_time) * 1000 >= (buffer[music_pos + 2] << 8) + buffer[music_pos + 3])
    {
        game_time = Timer::get_time();
        play((buffer[music_pos] << 8) + buffer[music_pos + 1]);
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