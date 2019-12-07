#ifndef SPEAKER_H
#define SPEAKER_H

#pragma once

class Speaker
{
public:
    static void play(unsigned long int freq);
    static void stop();
};

#endif