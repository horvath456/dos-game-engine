#ifndef SPEAKER_H
#define SPEAKER_H

#pragma once

namespace Speaker
{
void play_music(const char *play_string);
void stop_music();
void play(unsigned long int freq);
void stop();
} // namespace Speaker

#endif