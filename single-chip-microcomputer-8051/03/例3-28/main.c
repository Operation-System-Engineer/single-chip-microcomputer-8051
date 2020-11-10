#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */
#include <string.h>
#include <stdio.h>                /* prototype declarations for I/O functions */
#include <stdlib.h>
#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {
	char *ptr1,*ptr2,*temp;

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
	init_mempool (0x1000,0x500);   
	ptr1=malloc(30);/*动态为指针变量分配长度为30字节的存储空间*/
	ptr2=malloc(20) ;
	temp=malloc(30) ;
	printf("input str1:");
	gets(ptr1,20);/*输入字符串*/
	printf("input str2:");
	gets(ptr2,20) ;
	printf("str1------------str2\n") ;
	printf("%s.......%s\n",ptr1,ptr2) ;
	strcpy(temp,ptr1);/*串复制*/
	strcpy(ptr1,ptr2) ;
	strcpy(ptr2,temp) ;
	printf("str1------------str2\n") ;
	printf("%s.......%s\n",ptr1,ptr2) ;
	free(ptr1) ;
	free(ptr2) ;
    while (1) {};
}



