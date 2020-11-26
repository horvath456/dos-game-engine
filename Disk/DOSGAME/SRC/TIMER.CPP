#include "timer.h"

#include <dos.h>
#include <stdio>
#include <conio.h>
#include <set>
#include <vector>
#include <iostream>
#include <cstdio>
#include <iostream.h>

#define TIMEINT 0x8

namespace Timer
{
    void far __interrupt new_handler();
    void set_timer(int frate);
    void reset_timer();

    void far __interrupt (*old_handler)();
    long int clock_ticks;
    long int counter;
    unsigned long long game_time = 0;
    unsigned int frequency;
    std::set<void (*)(void)> callbacks;
    std::vector<void (*)(void)> delay_callbacks;
    std::vector<unsigned long long> delay_callback_times;

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

        std::set<void (*)(void)>::iterator it = callbacks.begin();
        while (it != callbacks.end())
        {
            ((void (*)(void))((*it)))();
            it++;
        }
        for (int i = delay_callbacks.size() - 1; i >= 0; i--)
        {
            if (delay_callback_times[i] <= game_time)
            {
                ((void (*)(void))(delay_callbacks[i]))();
                delay_callbacks.erase(delay_callbacks.begin() + i);
                delay_callback_times.erase(delay_callback_times.begin() + i);
            }
        }

        if (clock_ticks >= 0x10000)
        {
            clock_ticks -= 0x10000;
            old_handler();
        }
        else
        {
            outp(0x20, 0x20);
        }
    };

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

    unsigned int get_frequency()
    {
        return frequency;
    }

    void delay(unsigned long int ms)
    {
        unsigned long long start_time = game_time;
        while (get_delta_time(start_time) < ((double)(ms) / 1000.0))
            ;
    }

    void add_callback(void (*cb)(void))
    {
        callbacks.insert(cb);
    }

    void remove_callback(void (*cb)(void))
    {
        callbacks.erase(cb);
    }

    void add_timed_callback(void (*cb)(void), unsigned int ms)
    {
        unsigned long long call_game_time = game_time + (unsigned int)((double)frequency * ((double)ms / 1000.0));
        delay_callbacks.push_back(cb);
        delay_callback_times.push_back(call_game_time);
    }
} // namespace Timer