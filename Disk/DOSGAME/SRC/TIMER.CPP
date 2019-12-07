#include <dos.h>
#include <mem.h>

#include "timer.h"

Timer::Timer(unsigned int freq)
{
    Timer::set_timer(freq);
}

Timer::~Timer()
{
    reset_timer();
}

void far interrupt Timer::new_handler(...)
{
    game_time++;
    clock_ticks += counter;
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
    counter = 0x1234DD / frate;
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

unsigned long long Timer::get_time()
{
    return game_time;
}

void Timer::delay(unsigned long int ms)
{
    unsigned long long start_timer = get_time();
    while (get_time() - start_timer < ms)
    {
    }
}

void far interrupt (*Timer::old_handler)(...);
long int Timer::clock_ticks, Timer::counter;
unsigned long long Timer::game_time;