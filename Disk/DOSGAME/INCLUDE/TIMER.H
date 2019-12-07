#ifndef TIMER_H
#define TIMER_H

#pragma once

#include <dos.h>

#define TIMEINT 0x8
#define FRATE 10

// Timer fires every 5.5 ms

class Timer
{
public:
    Timer();

    ~Timer();

private:
    static void far interrupt (*old_handler)(...);
    static long int clock_ticks, counter;
    static unsigned long int game_time;

    static void far interrupt new_handler(...);

    static void set_timer(int frate);
    static void reset_timer();
};

#endif