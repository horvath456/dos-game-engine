#include "speaker.h"

#include <dos.h>

//Play sound using built in speaker
void Speaker::play(unsigned long int freq)
{
    //Set the PIT to the desired frequency
    unsigned long int div = 1193180 / freq;
    outp(0x43, 0xb6);
    outp(0x42, (unsigned char)(div));
    outp(0x42, (unsigned char)(div >> 8));

    //And play the sound using the PC speaker
    unsigned char tmp = inp(0x61);
    if (tmp != (tmp | 3))
    {
        outp(0x61, tmp | 3);
    }
}

//make it shutup
void Speaker::stop()
{
    unsigned char tmp = inp(0x61) & 0xFC;
    outp(0x61, tmp);
}
