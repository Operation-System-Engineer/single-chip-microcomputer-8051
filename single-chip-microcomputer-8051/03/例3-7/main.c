#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */

#define MAX 100
#define LEN 80
char text[MAX][LEN];
										 
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
/* 一个非常简单的文本编辑器*/
void main (void) {
	register int t ,i ,j ;
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

	for(t=0;t<MAX; t++) /*逐行输入字符串*/
	{
		gets(text[t],LEN);
		if(!text[t][0]) break; /* 空行退出*/
	}
	
	for(i=0;i<t;i++)
	{/*按行,逐个字符输出字符串*/
		for(j=0; text [i][j];j++)
		putchar(text [i][j]);
		putchar( '\n');
	}
    while (1) {};
}



