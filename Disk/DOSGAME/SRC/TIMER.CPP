#include "timer.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <string.h>

#define TIMEINT 0x8

namespace Timer
{
void far __interrupt new_handler();
void set_timer(int frate);
void reset_timer();

void far __interrupt (*old_handler)();
long int clock_ticks, counter;
unsigned long long game_time;
unsigned int frequency;
void (*callbacks[10])(void *);
void *callback_p_arguments[10];
void (*delay_callbacks[10])(void *);
unsigned long long delay_callback_times[10];
void *delay_callback_p_arguments[10];

void init_timer(unsigned int freq)
{
    frequency = freq;
    set_timer(freq);
}

void deinit_timer()
{
    reset_timer();
}

void far __interrupt new_handler()
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
            if (delay_callbacks[i] != (void (*)(void *))0x0)
            {
                if (delay_callback_times[i] <= game_time)
                {
                    ((void (*)(void *))(delay_callbacks[i]))(delay_callback_p_arguments[i]);
                    delay_callbacks[i] = 0x0;
                    delay_callback_times[i] = 0;
                    delay_callback_p_arguments[i] = 0x0;
                }
            }
        }
        outp(0x20, 0x20);
    };
}

void set_timer(int frate)
{
    clock_ticks = 0;
    counter = 0x1234DD / frate;
    old_handler = _dos_getvect(TIMEINT);
    _dos_setvect(TIMEINT, new_handler);
    outp(0x43, 0x34);
    outp(0x40, counter % 256);
    outp(0x40, counter / 256);
}

void reset_timer()
{
    outp(0x43, 0x34);
    outp(0x40, 0);
    outp(0x40, 0);
    _dos_setvect(TIMEINT, old_handler);
}

unsigned long long get_time()
{
    return game_time;
}

double get_delta_time(unsigned long long t)
{
    return (double)((double)(game_time - t) / (double)frequency);
}

void delay(unsigned long int ms)
{
    unsigned long long start_timer = game_time;
    while ((double)((double)(game_time - start_timer) / (double)frequency) < ((double)(ms) / 1000.0))
    {
    }
}

void add_callback(void (*cb)(void *), void *p)

{
    for (int i = 0; i < 10; i++)
    {
        if (callbacks[i] == 0x0)
        {
            callbacks[i] = cb;
            callback_p_arguments[i] = p;
            return;
        }
    }
}

void remove_callback(void (*cb)(void *))
{
    for (int i = 0; i < 10; i++)
    {
        if (callbacks[i] == cb)
        {
            callbacks[i] = 0x0;
            callback_p_arguments[i] = 0x0;
        }
        return;
    }
}

int add_timed_callback(void (*cb)(void *), unsigned int ms, void *p)
{
    for (int i = 0; i < 10; i++)
    {
        if (delay_callbacks[i] == 0x0)
        {
            delay_callbacks[i] = cb;
            delay_callback_times[i] = game_time + (unsigned int)((double)frequency * ((double)ms / 1000.0));
            delay_callback_p_arguments[i] = p;
            return i;
        }
    }
    return -1;
}

void remove_timed_callback(int handle)
{
    delay_callbacks[handle] = 0x0;
    delay_callback_times[handle] = 0x0;
    delay_callback_p_arguments[handle] = 0x0;
}
} // namespace Timer