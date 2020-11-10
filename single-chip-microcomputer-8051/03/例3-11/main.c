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
	void change(int *pt1,int *pt2); /*函数声明*/
	int *p1,*p2,a,b;

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
	scanf("%d ,%d" ,&a, &b);
	p1 =&a;
	p2 =&b;
	change(p1, p2); /*子程序调用*/
	printf("%d ,%d \n",*p1,*p2);

    while (1) {};
}

void change(int *pt1,int *pt2)
{ /*子程序实现将两数值调整为由大到小*/
	int t;
	if (*pt1<*pt2) /*交换内存变量的值*/
	{
		t=*pt1; *pt1=*pt2; *pt2=t; 
	}
	return ;
}



