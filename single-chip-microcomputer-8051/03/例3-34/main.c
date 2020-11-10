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

	int a[10],b[3][4],*p1,*p2,**p3,i,j;/*p3 是指向指针的指针变量*/
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
	for( i = 0 ; i < 10 ; i++)
		scanf( "%d", &a[i]) ;/*一维数组的输入*/
	for (i=0;i<3;i++)
		for( j = 0 ; j < 4 ; j++ )
			scanf( "%d" , &b[i][j]) ;/*二维数组输入*/
	for (p1=a,p3=&p1,i=0;i<10;i++)
		printf( "%4d" , *(*p3+i) ) ;/*用指向指针的指针变量输出一维数组*/
	printf( "\n" ) ;
	for (p1=a;p1-a<10;p1++)/* 用指向指针的指针变量输出一维数组*/
		{
			p3 =&p1 ;
			printf("%4d",**p3);
		}
	printf( "\n" ) ;
	for(i=0;i<3;i++)/* 用指向指针的指针变量输出二维数组*/
		{
			p2=b[i];
			p3=&p2;
			for (j=0;j<4;j++)
				printf("%4d",*(*p3+j));
			printf("\n");
		}
	for(i= 0 ; i < 3 ;i++)/*用指向指针的指针变量输出二维数组*/
		{
			p2 =b[i];
			for(p2 =b[i] ; p2-b[i]<4;p2++)
			{
				p3 =&p2;
				printf("%4d",**p3) ;
			}
			printf("\n");
		}
    while (1) {};
}


