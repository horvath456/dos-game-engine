#ifndef SPRSHEET_H
#define SPRSHEET_H

#pragma once

class Spritesheet
{
public:
    Spritesheet(const char *filename);
    ~Spritesheet();

    unsigned char far *get_buffer_at(unsigned int x, unsigned int y);
    unsigned char pixel_at(unsigned int x, unsigned int y);

    unsigned int width;
    unsigned int height;

private:
    unsigned char far *buffer;
};

#endif