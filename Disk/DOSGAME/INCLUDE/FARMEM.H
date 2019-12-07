#ifndef FARMEM_H
#define FARMEM_H

#pragma once

void fmemcpy(unsigned char far *source, unsigned char far *dst, unsigned int size);

void fmemset(unsigned char far *buf, unsigned int size, unsigned char value);

#endif