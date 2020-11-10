#include "reg51.h"

#define ON  1
#define OFF 0

sbit Lamp1=P0^0;
sbit Lamp2=P0^1;
sbit Lamp3=P0^0;
sbit Lamp4=P0^1;
sbit Lamp5=P0^0;
sbit Lamp6=P0^1;
sbit Lamp7=P0^0;
sbit Lamp8=P0^1;

void initial();
void delay();

main()
{
    initial();
    delay();
    while(1)
    {
        Lamp1=ON;
        delay();
        Lamp1=OFF;
        delay();
        
        Lamp2=ON;
        delay();
        Lamp2=OFF;
        delay();
        
        Lamp3=ON;
        delay();
        Lamp3=OFF;
        delay();
        
        Lamp4=ON;
        delay();
        Lamp4=OFF;
        delay();
        Lamp5=ON;
        delay();
        Lamp5=OFF;
        delay();
        
        Lamp6=ON;
        delay();
        Lamp6=OFF;
        delay();
        
        Lamp7=ON;
        delay();
        Lamp7=OFF;
        delay();
        
        Lamp8=ON;
        delay();
        Lamp8=OFF;
        delay();
        
    }
}
void initial()
{
    Lamp1=OFF;
    Lamp2=OFF;
    Lamp3=OFF;
    Lamp4=OFF;
    Lamp5=OFF;
    Lamp6=OFF;
    Lamp7=OFF;
    Lamp8=OFF;
}

void delay()
{
    int i=0;
    while(i<10000)i++;
}

