#ifndef ENGINE_ADLIB_H
#define ENGINE_ADLIB_H

#pragma once

namespace Engine
{
    namespace Adlib
    {
        void init_adlib();
        void reset_card();

        void play_music(const char *filename, unsigned int rate, bool loop = true);
        void stop_music();

        bool check_if_adlib_present();

        void play(unsigned char channel, unsigned int freq, unsigned char octave, unsigned char waveform);
        void stop(unsigned char channel);
    }
};

#endif // ENGINE_ADLIB_H