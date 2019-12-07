#ifndef FARMEM_H
#define FARMEM_H

void fmemcpy(unsigned char far *source, unsigned char far *dst, unsigned int size)
{
    while (size-- != 0)
        *dst++ = *source++;
}

void fmemset(unsigned char far *buf, unsigned int size, unsigned char value)
{
     while (size-- != 0)
        *buf++ = value;
}

#endif