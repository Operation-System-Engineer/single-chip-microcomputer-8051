#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
struct time
{
	int year;/*年*/
	int month;/*月*/
	int day;/*日*/
} ;
union dig
{
	struct time data1;/*嵌套的结构体类型*/
	char byte[6];
} ;
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	union dig unit;
	int i;
/*------------------------------------------------
Setup the serial port for 1200 baud at 16MHz.
------------------------------------------------*/
#ifndef MONITOR51
    SCON  = 0x50;		        /* SCON: mode 1, 8-bit UART, enable rcvr      */
    TMOD |= 0x20;               /* TMOD: timer 1, mode 2, 8-bit reload        */
    TH1   = 221;                /* TH1:  reload value for 1200 baud @ 16MHz   */
    TR1   = 1;                  /* TR1:  timer 1 run                          */
    TI    = 1;                  /* TI:   set TI to send first char of UART    */
#endif
/*------------------------------------------------
Note that an embedded program never exits (because
there is no operating system to return to).  It
must loop and execute forever.
------------------------------------------------*/
	printf("enter year:\n");
	scanf("%d",&unit.data1.year);/*输入年*/
	printf("enter month:\n");
	scanf("%d",&unit.data1.month);/*输入月*/
	printf("enter day:\n");
	scanf("%d",&unit.data1.day);/*输入日*/
	printf("year=%dmnh%day=%d\n",unit.data1.year,unit.data1.month,unit.data1.day);/*打印输出*/
	for(i=0;i<6;i++)
		printf("%d,",unit.byte[i]);/*按字节以十进制输出*/
	printf("\n");

    while (1) {};
}



