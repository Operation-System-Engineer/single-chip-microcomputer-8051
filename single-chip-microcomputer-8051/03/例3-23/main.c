#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
#define M 3
#define N 4
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	float a[M][N];
	float score1,score2,score3, *pa=a[0];/*指针变量p a 指向二维数组*/
	/* score1,score2,score3 分别记录三行的数据相加*/
	int i,j;
	void fun(float b[][N],float *p1,float *p2,float *p3);
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
	for( i = 0 ; i < M ; i++)
		for(j=0;j<N; j++)/*二维数组的数据输入*/
			scanf("%f",&a[i][j]) ;
		fun(pa,&score1,&score2,&score3) ;
		/*函数调用，不仅传递数组首地址，还要传递变量的地址*/
		printf("%.2f,%.2f,%.2f\n",score1,score2,score3);
    while (1) {};
}



void fun(float b[][N],float *p1,float *p2,float *p3)
{
	int i,j;
	*p1=*p2=*p3=0;
	for(i=0; i < M ; i++)
		for ( j = 0 ; j < N ; j++)
		{
			if(i==0) *p1=*p1+b[i][j];/*第0 行的数据相加*/
			if(i==1) *p2=*p2+b[i][j];/*第1 行的数据相加*/
			if(i==2) *p3=*p3+b[i][j];/*第2 行的数据相加*/
		}
}

