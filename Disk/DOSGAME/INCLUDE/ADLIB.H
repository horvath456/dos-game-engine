#ifndef ADLIB_H
#define ADLIB_H

#pragma once

#include "timer.h"

#define ADDRESS_STATUS_PORT 0x388
#define DATA_PORT 0x389

#define TRUE 1
#define FALSE 0

class Adlib
{
public:
    static void init_adlib();
    static void reset_card();

    static void play_music(const char *filename, int loop = 0);
    static void stop_music();

    static int check_if_adlib_present();

    static void play(unsigned char channel, unsigned int freq, unsigned char octave, unsigned char waveform);

    static void stop(unsigned char channel);

private:
    static void callback_handler(void *_);
    static void play_music_end();

    static unsigned char far *buffer;
    static int is_music_playing;
    static unsigned int music_pos;
    static unsigned long long game_time;
    static unsigned long int buffer_length;
    static Timer *timer;
    static Adlib *self;
    static int music_looping;

    static int is_present;

    static void write_register(unsigned char num, unsigned char value);
    static unsigned char read_status_register();
    static void delay_using_read(unsigned int iterations);

    static unsigned char calc_register_offset(unsigned char channel);
};

#endif