#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <stdlib.h>/*ʹ��malloc( )��Ҫ*/

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
struct data1 /*����ṹ��*/
{
	int day,month,year;
};
struct stu/*����ṹ��*/
{
	char name[20];
	long num;
	struct data1 birthday;/*Ƕ�׵Ľṹ�����ͳ�Ա*/
} ;
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	struct stu *student;/*����ṹ������ָ��*/
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
	init_mempool(0x1000,0x500);
	student=malloc(sizeof(struct stu));/*Ϊָ��������䰲ȫ�ĵ�ַ*/
	printf("Input name,number,year,month,day:\n");
	scanf("%s",student->name);/*����ѧ��������ѧ�š�����������*/
	scanf("%ld",&student->num) ;
	scanf("%d%d%d",&student->birthday.year,&student->birthday.month,&student->birthday.day);
	printf("\nOutput name,number,year,month,day\n" );
	/*��ӡ�������Ա���ֵ*/
		printf("%20s%10ld%10d//%d//%d\n",student->name,student->num,
		student->birthday.year,student->birthday.month,
		student->birthday.day) ;
    while (1) {};
}



