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
	/* max ����������maxi�����Ԫ�������У�maxj�����Ԫ��������*/
	int max_arr(int *b,int *p1,int *p2,int n);/*��������*/
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
		ptr= a[0] ;/*����ά������׵�ַ���ݸ�ָ�����*/
		max_arr(ptr, &max,&maxi,12) ;
		maxj=maxi%4;/*ÿ�����ĸ�Ԫ�أ����Ԫ��������*/
		maxi=maxi/4 ;/*���Ԫ��������*/
		printf("max=%d,maxi=%d,maxj=%d",max,maxi,maxj);
    while (1) {};
}

int max_arr(int *b,int *p1,int *p2,int n)
/* b ָ���ά�����ָ�룬p1ָ�����ֵ��p2ָ�����ֵ��һά�����е�λ�ã�*/
/* n ������Ĵ�С*/
{
	int i;
	*p1=b[0]; *p1=0;
	for(i=1;i < n;i++)/*�����*/
		if (b[i]>*p1) {*p1=b[i]; *p2=i;}
}


