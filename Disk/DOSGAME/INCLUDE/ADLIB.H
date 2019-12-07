#ifndef ADLIB_H
#define ADLIB_H

#pragma once

#include "timer.h"

#define ADDRESS_STATUS_PORT 0x388
#define DATA_PORT 0x389

class Adlib
{
public:
    Adlib();
    ~Adlib();

    static int check_if_adlib_present();

    void play();

    void stop();

private:
    static void write_register(unsigned char num, unsigned char value);
    static unsigned char read_status_register();
    static void delay_using_read(unsigned int iterations);
    static void reset_card();
};

#endif