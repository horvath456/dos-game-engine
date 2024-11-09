#ifndef ENGINE_SPEAKER_H
#define ENGINE_SPEAKER_H

#pragma once

namespace Engine
{
    namespace Speaker
    {
        void play_sfx(unsigned int far *data, unsigned int length);
        void stop_sfx();
        void play(unsigned long int freq);
        void stop();
    };
};

#endif // ENGINE_SPEAKER_H