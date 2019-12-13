#ifndef FONT_H
#define FONT_H

#pragma once

class Font
{
public:
    Font(const char *filename);
    ~Font();

    unsigned char far *get_buffer(char c);

    unsigned char width;
    unsigned char height;

private:
    unsigned char far *buffer;
};

#endif