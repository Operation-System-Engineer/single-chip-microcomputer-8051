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

	char *ptr1[4]={"china","chengdu","sichuang","chongqin"};
	/* 指针数组ptr1 的4 个指针分别依此指向4个字符串*/
	int i,*ptr2[3],a[3]={1,2,3},b[3][2]={1,2,3,4,5,6};
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
	for (i=0;i<4;i++)
		printf("\n%s",ptr1[i]);/*依此输出ptr1数组4个指针指向的4个字符串*/
	printf("\n") ;
	for(i=0;i<3;i++)
		ptr2[i]=&a[i];/*将整型一维数组a的3个元素的地址传递给指针数组ptr2*/
	for(i=0;i<3;i++)/*依此输出ptr2所指向的3个整型变量的值*/
		printf("%4d",*ptr2[i]) ;
	printf("\n");
	for(i=0;i<3;i++)
		ptr2[i]=b[i];/*传递二维数组b的每行首地址给指针数组的4个指针*/
	for(i=0;i<3;i++)/*按行输出*/
		printf("%4d%4d\n",*ptr2[i],*ptr2[i]+1);
    while (1) {};
}



