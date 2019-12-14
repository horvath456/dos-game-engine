#ifndef SPEAKER_H
#define SPEAKER_H

#pragma once

namespace Speaker
{
void play_music_string(const char *play_string, bool loop_music = false);
void play_music(const char *filename, bool loop_music = false);
void stop_music();
void play(unsigned long int freq);
void stop();
} // namespace Speaker

#endif