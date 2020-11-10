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

	char str1[30],str2[20],*ptr1=str1,*ptr2=str2;
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
	printf("input str1:");
	gets(str1,20);/*����str1*/
	printf("input str2:");
	gets(str2,20);/*����str2*/
	printf("str1 - - - - - - - - - - - - str2\n") ;
	printf("%s.......%s\n",ptr1,ptr2) ;
	while(*ptr2) *ptr1++=*ptr2++;/*�ַ�������*/
	*ptr1='\0';/*д�봮�Ľ�����־*/
	printf("str1 - - - - - - - - - - - - str2\n" ) ;
	printf("%s . . . . . . . %s\n",str1,str2) ;
    while (1) {};
}



