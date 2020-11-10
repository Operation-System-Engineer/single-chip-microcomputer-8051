#include "reg51.h"

sbit CLR=P0^0;
sbit READY=P0^1;

void delay();

main()
{
    unsigned char indata;
    CLR=0;
    while(1)
    {
        while(READY)
        {
            indata=P1;
            CLR=1;
            delay();
            CLR=0;
        }
    }
}

void delay()
{
    int i=0;
    while(i<100)i++;
}
