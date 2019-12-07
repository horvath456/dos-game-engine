#ifndef TIMER_H
#define TIMER_H

#pragma once

#include <dos.h>

#define TIMEINT 0x8

class Timer
{
public:
    Timer(unsigned int freq);

    ~Timer();

    unsigned long long get_time();

    void delay(unsigned long int ms);

private:
    static void far interrupt (*old_handler)(...);
    static long int clock_ticks, counter;
    static unsigned long long game_time;

    static void far interrupt new_handler(...);

    static void set_timer(int frate);
    static void reset_timer();
};

#endif