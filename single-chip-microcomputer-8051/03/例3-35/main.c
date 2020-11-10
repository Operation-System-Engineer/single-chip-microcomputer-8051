#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	int i;
	static char c[][16]={"clanguage","fox","computer","home page"};
	/*二维字符数组*/
	static char *cp[]={c[0],c[1],c[2],c[3]};/* 指针数组*/
	static char **cpp;/* 指向字符指针的指针变量*/
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
	cpp=cp;/*将指针数组的首地址传递给指向字符指针的指针变量*/
	for (i=0;i<4;i++)/*按行输出字符串*/
		printf("%s\n",*cpp++) ;
	printf( " - - - - - - - - - - - \n " ) ;
	for (i=0;i<4;i++)/*按行输出字符串*/
	{
		cpp=&cp[i] ;
		printf("%s\n",*cpp) ;
	}
    while (1) {};
}



