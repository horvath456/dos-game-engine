#ifndef TIMER_H
#define TIMER_H

#pragma once

namespace Timer
{
void init_timer(unsigned int freq);
void deinit_timer();

unsigned long long get_time();
double get_delta_time(unsigned long long d);
unsigned int get_frequency();

void delay(unsigned long int ms);

void add_callback(void (*cb)(void));
void remove_callback(void (*cb)(void));

void add_timed_callback(void (*cb)(void), unsigned int ms);
}; // namespace Timer

#endif