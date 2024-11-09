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

namespace Engine
{
    namespace Speaker
    {

        void callback_handler();
        void play_music_end();

        unsigned int far *buffer = NULL;
        bool is_music_playing = false;

        unsigned int music_pos = 0;
        unsigned int wait_time = 0;
        unsigned long long game_time = 0;
        unsigned int buffer_length = 0;

        void Speaker::play_sfx(unsigned int far *data, unsigned int length)
        {
            stop_sfx();

            is_music_playing = true;
            music_pos = 0;
            wait_time = 0;
            game_time = Timer::get_time();
            buffer_length = length;
            buffer = data;

            Timer::add_callback(&callback_handler);
        }

        void Speaker::stop_sfx()
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
            buffer = NULL;
        }

        void Speaker::callback_handler()
        {
            if (Timer::get_delta_time(game_time) * 1000 >= wait_time)
            {
                unsigned int note = buffer[music_pos];
                wait_time = buffer[music_pos + 1];

                game_time = Timer::get_time();

                if (note > 0)
                    play(note);
                else
                    stop();

                music_pos += 2;
            }

            if (music_pos >= buffer_length)
                play_music_end();
        }

        // Play sound using built in speaker
        void Speaker::play(unsigned long int freq)
        {
            // Set the PIT to the desired frequency
            unsigned long int div = 1193180 / freq;
            outp(0x43, 0xb6);
            outp(0x42, (unsigned char)(div));
            outp(0x42, (unsigned char)(div >> 8));

            // And play the sound using the PC speaker
            unsigned char tmp = inp(0x61);
            if (tmp != (tmp | 3))
            {
                outp(0x61, tmp | 3);
            }
        }

        // make it shutup
        void Speaker::stop()
        {
            unsigned char tmp = inp(0x61) & 0xFC;
            outp(0x61, tmp);
        }
    } // namespace Speaker
} // namespace Engine