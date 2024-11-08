#ifndef SPEAKER_H
#define SPEAKER_H

#pragma once

namespace Speaker
{
    void play_sfx(unsigned int far *data, unsigned int length);
    void stop_sfx();
    void play(unsigned long int freq);
    void stop();
} // namespace Speaker

#endif