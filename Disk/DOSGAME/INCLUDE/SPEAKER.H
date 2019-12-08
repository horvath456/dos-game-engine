#ifndef SPEAKER_H
#define SPEAKER_H

#pragma once

#include "timer.h"

class Speaker
{
public:
    static void play_music(const char *filename, Timer &timer);
    static void stop_music();
    static void play(unsigned long int freq);
    static void stop();

private:
    static void callback_handler();
    static void play_music_end();

    static unsigned char far *buffer;
    static int is_music_playing;
    static unsigned int music_pos;
    static unsigned long long game_time;
    static unsigned long int buffer_length;
    static Timer* timer;
};

#endif