#ifndef TIMER_H
#define TIMER_H

#pragma once

#define TIMEINT 0x8

class Timer
{
public:
    static void init_timer(unsigned int freq);
    static void deinit_timer();

    static unsigned long long get_time();
    static double get_delta_time(unsigned long long d);

    static void delay(unsigned long int ms);

    static void add_callback(void (*cb)(void *), void *p);
    static void remove_callback(void (*cb)(void *));

    static int add_timed_callback(void (*cb)(void *), unsigned int ms, void *p);
    static void remove_timed_callback(int handle);

private:
    static void far __interrupt (*old_handler)();
    static long int clock_ticks, counter;
    static unsigned long long game_time;
    static unsigned int frequency;

    static void (*callbacks[10])(void *);
    static void *callback_p_arguments[10];

    static void (*delay_callbacks[10])(void *);
    static unsigned long long delay_callback_times[10];
    static void *delay_callback_p_arguments[10];

    static void far __interrupt new_handler();

    static void set_timer(int frate);
    static void reset_timer();
};

#endif