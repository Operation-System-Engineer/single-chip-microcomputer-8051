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
	int sub_max(int b[],int i);/*函数声明*/
	int n,a[10],*ptr=a;/*定义变量，并使指针指向数组*/
	int max;

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
	for( n = 0 ; n <= 9 ; n++ )/*输入数据*/
		scanf ( "%d" , &a[n] ) ;
	max=sub_max(ptr,10);/*函数调用，其实参是指针*/
	printf( "max=%d\n" , max ) ;
    while (1) {};
}

int sub_max(int b[],int i)/*函数定义，其形参为数组*/
{
	int temp,j;
	temp= b[0];
	for( j = 1 ; j <= i-1 ; j++ )
		if(temp<b[j]) temp=b[j];
		return temp;
}


