#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <stdlib.h>
#include <string.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	char *ptr1[4],*temp;
	int i,j;
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
	for (i=0;i<4;i++)
	{
		ptr1[i] = malloc(20) ;/*为指针数组各指针分配20字节的存储空间*/
		gets(ptr1[i],20) ;
	}
	printf("\n") ;
	printf("original string:\n");
	for(i=0;i<4;i++)
		printf("%s\n",ptr1[i]) ;
	printf("ordinal string:\n");
	for( i = 0 ; i < 3 ; i++)
		for( j = 0 ; j < 4 - i - 1 ; j++)
			if(strcmp(ptr1[j],ptr1[j+1])>0)
			{
				temp=ptr1[j];/*利用指向字符串的指针，进行指针地址的交换*/
				ptr1[j]=ptr1[j+1] ;
				ptr1[j+1]=temp;
			}
	for( i=0;i<4;i++)/*字符串输出*/
		printf("%s\n" , ptr1[i]);
    while (1) {};
}



