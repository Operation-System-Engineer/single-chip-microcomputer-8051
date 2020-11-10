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
	/* ָ������ptr1 ��4 ��ָ��ֱ�����ָ��4���ַ���*/
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
		printf("\n%s",ptr1[i]);/*�������ptr1����4��ָ��ָ���4���ַ���*/
	printf("\n") ;
	for(i=0;i<3;i++)
		ptr2[i]=&a[i];/*������һά����a��3��Ԫ�صĵ�ַ���ݸ�ָ������ptr2*/
	for(i=0;i<3;i++)/*�������ptr2��ָ���3�����ͱ�����ֵ*/
		printf("%4d",*ptr2[i]) ;
	printf("\n");
	for(i=0;i<3;i++)
		ptr2[i]=b[i];/*���ݶ�ά����b��ÿ���׵�ַ��ָ�������4��ָ��*/
	for(i=0;i<3;i++)/*�������*/
		printf("%4d%4d\n",*ptr2[i],*ptr2[i]+1);
    while (1) {};
}



