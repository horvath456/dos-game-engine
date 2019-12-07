#include <dos.h>
#include <mem.h>

#include "timer.h"

#define TIMEINT 0x8

Timer::Timer(int frate)
{
    Timer::game_time = 0;
    Timer::set_timer(frate);
}

Timer::~Timer()
{
    reset_timer();
}

void far interrupt Timer::new_handler(...)
{
    clock_ticks += counter;
    game_time++;
    if (clock_ticks >= 0x10000)
    {
        clock_ticks -= 0x10000;
        old_handler();
    }
    else
    {
        outport(0x20, 0x20);
    };
}

void Timer::set_timer(int frate)
{
    clock_ticks = 0;
    counter = 0x10000 / frate;
    old_handler = getvect(TIMEINT);
    setvect(TIMEINT, Timer::new_handler);
    outport(0x43, 0x34);
    outport(0x40, counter % 256);
    outport(0x40, counter / 256);
}

void Timer::reset_timer()
{
    outport(0x43, 0x34);
    outport(0x40, 0);
    outport(0x40, 0);
    setvect(TIMEINT, old_handler);
}

void far interrupt (*Timer::old_handler)(...);
long int Timer::clock_ticks, Timer::counter;
unsigned long int Timer::game_time;