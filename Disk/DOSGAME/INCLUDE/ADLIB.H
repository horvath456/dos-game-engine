#ifndef ADLIB_H
#define ADLIB_H

#pragma once

#define ADDRESS_STATUS_PORT 0x388
#define DATA_PORT 0x389

class Adlib
{
public:
    Adlib();
    ~Adlib();

    static int check_if_adlib_present();

    void play(unsigned char channel, unsigned int freq, unsigned char octave, unsigned char waveform);

    void stop(unsigned char channel);

private:
    static void write_register(unsigned char num, unsigned char value);
    static unsigned char read_status_register();
    static void delay_using_read(unsigned int iterations);
    static void reset_card();
    static void init();
    static unsigned char calc_register_offset(unsigned char channel);
};

#endif