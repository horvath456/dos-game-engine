#ifndef TIMER_H
#define TIMER_H

#pragma once

namespace Timer
{
    void init_timer(unsigned int freq);
    void deinit_timer();

    unsigned long long get_time();
    double get_delta_time(unsigned long long d);

    void delay(unsigned long int ms);

    void add_callback(void (*cb)(void *), void *p);
    void remove_callback(void (*cb)(void *));

    int add_timed_callback(void (*cb)(void *), unsigned int ms, void *p);
    void remove_timed_callback(int handle);
};

#endif