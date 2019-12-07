#include <dos.h>
#include <time.h>

#include "adlib.h"

Adlib::Adlib()
{
    reset_card();
}

Adlib::~Adlib()
{
    reset_card();
}

void Adlib::play()
{
    write_register(0x20, 0x01);
    write_register(0x40, 0x10);
    write_register(0x60, 0xF0);
    write_register(0x80, 0x77);
    write_register(0xA0, 0x98);
    write_register(0x23, 0x01);
    write_register(0x43, 0x00);
    write_register(0x63, 0xF0);
    write_register(0x83, 0x77);
    write_register(0xB0, 0x31);
}

void Adlib::stop()
{
    delay_using_read(35);
    write_register(0xB0, 0x11);
}

int Adlib::check_if_adlib_present()
{
    write_register(4, 0x60);
    write_register(4, 0x80);
    unsigned char status1 = read_status_register();
    write_register(2, 0xFF);
    write_register(4, 0x21);
    delay_using_read(800);
    unsigned char status2 = read_status_register();
    write_register(4, 0x60);
    write_register(4, 0x80);
    return (((status1 & 0xE0) == 0x0) && ((status2 & 0xE0) == 0x0C0));
}

void Adlib::write_register(unsigned char num, unsigned char value)
{
    outp(ADDRESS_STATUS_PORT, num);
    delay_using_read(6);
    outp(DATA_PORT, value);
    delay_using_read(35);
}

unsigned char Adlib::read_status_register()
{
    return inp(ADDRESS_STATUS_PORT);
}

void Adlib::delay_using_read(unsigned int iterations)
{
    for (int i = 0; i < iterations; i++)
    {
        read_status_register();
    }
}

void Adlib::reset_card()
{
    for (unsigned char i = 0x01; i <= 0xF5; i++)
    {
        write_register(i, 0x0);
    }
}