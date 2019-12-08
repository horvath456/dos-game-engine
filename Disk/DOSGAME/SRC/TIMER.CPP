#include "timer.h"

#include <dos.h>
#include <mem.h>

Timer::Timer(unsigned int freq)
{
    frequency = freq;
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
        for (int i = 0; i < 10; i++)
        {
            if (callbacks[i] != (void (*)(void *))0x0)
            {
                ((void (*)(void *))(callbacks[i]))(callback_p_arguments[i]);
            }
            if (delay_callbacks[i] != (void (*)(void *))0x0 && delay_callback_times[i] >= game_time)
            {
                ((void (*)(void *))(delay_callbacks[i]))(delay_callback_p_arguments[i]);
                delay_callbacks[i] = (void *)0x0;
                delay_callback_times[i] = 0;
                delay_callback_p_arguments[i] = (void *)0x0;
            }
        }
        outport(0x20, 0x20);
    };
}

void Timer::set_timer(int frate)
{
    clock_ticks = 0;
    counter = 0x1234DD / frate;
    old_handler = getvect(TIMEINT);
    setvect(TIMEINT, new_handler);
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

double Timer::get_delta_time(unsigned long long t)
{
    return (double)((double)(game_time - t) / (double)frequency);
}

void Timer::delay(unsigned long int ms)
{
    unsigned long long start_timer = game_time;
    while ((double)((double)(game_time - start_timer) / (double)frequency) < ms)
    {
    }
}

void Timer::add_callback(void (*cb)(void *), void *p)

{
    for (int i = 0; i < 10; i++)
    {
        if (callbacks[i] == (void (*)(void *))0x0)
        {
            callbacks[i] = cb;
            callback_p_arguments[i] = p;
            return;
        }
    }
}

void Timer::remove_callback(void (*cb)(void *))
{
    for (int i = 0; i < 10; i++)
    {
        if (callbacks[i] == (void (*)(void *))cb)
        {
            callbacks[i] = (void *)0x0;
            callback_p_arguments[i] = (void *)0x0;
        }
        return;
    }
}

int Timer::add_timed_callback(void (*cb)(void *), unsigned int ms, void *p)
{
    for (int i = 0; i < 10; i++)
    {
        if (delay_callbacks[i] == (void (*)(void *))0x0)
        {
            delay_callbacks[i] = cb;
            delay_callback_times[i] = game_time + ms * frequency;
            delay_callback_p_arguments[i] = p;
            return i;
        }
    }
    return -1;
}

void Timer::remove_timed_callback(int handle)
{
    delay_callbacks[handle] = (void *)0x0;
    delay_callback_times[handle] = 0x0;
    delay_callback_p_arguments[handle] = (void *)0x0;
}

void far interrupt (*Timer::old_handler)(...);
long int Timer::clock_ticks, Timer::counter;
unsigned long long Timer::game_time;
unsigned int Timer::frequency;
void (*Timer::callbacks[10])(void *);
void *Timer::callback_p_arguments[10];
void (*Timer::delay_callbacks[10])(void *);
unsigned long long Timer::delay_callback_times[10];
void *Timer::delay_callback_p_arguments[10];