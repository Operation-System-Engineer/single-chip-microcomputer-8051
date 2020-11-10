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

	int a[3][4],*ptr,i,j,max,maxi,maxj;
	/* max 是数组的最大，maxi是最大元素所在行，maxj是最大元素所在列*/
	int max_arr(int *b,int *p1,int *p2,int n);/*函数声明*/
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
	for( i = 0 ; i < 3 ; i++)
		for( j = 0 ; j < 4 ; j++)
			scanf( "%d" , &a[i][j]) ;
		ptr= a[0] ;/*将二维数组的首地址传递给指针变量*/
		max_arr(ptr, &max,&maxi,12) ;
		maxj=maxi%4;/*每行有四个元素，求该元素所在列*/
		maxi=maxi/4 ;/*求该元素所在行*/
		printf("max=%d,maxi=%d,maxj=%d",max,maxi,maxj);
    while (1) {};
}

int max_arr(int *b,int *p1,int *p2,int n)
/* b 指向二维数组的指针，p1指向最大值，p2指向最大值在一维数组中的位置，*/
/* n 是数组的大小*/
{
	int i;
	*p1=b[0]; *p1=0;
	for(i=1;i < n;i++)/*找最大*/
		if (b[i]>*p1) {*p1=b[i]; *p2=i;}
}


