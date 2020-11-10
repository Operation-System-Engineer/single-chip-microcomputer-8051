#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <math.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */


/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	float a,b,c,s,x1,x2;
	double t; 
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
	printf(" please input a,b,c:");
	scanf("%f %f %f ",&a,&b,&c) ;
	if(a==0.0)
		if(b!=0.0)
			printf("the root is :%f\n",-c/b);
		else if(c==0.0)
			printf("x is inexactive\n ");
		else
			printf("no root!\n");
	else
	{
		s=b*b-4*a*c;
		if(s>=0.0)
			if(s>0.0)
			{
				t=sqrt(s);
				x1=-0.5*(b+t)/a;
				x2=-0.5*(b-t)/a;
				printf("There are two different roots:%f and %f\n",x1,x2);
			}
			else
				printf("There are two equal roots:%f\n",-0.5*b/a);
		else
		{
			t=sqrt(-s) ;
			x1=-0.5*b/a; /*实部*/
			x2=abs(0.5*t/a); /*虚部的绝对值*/
			printf("There are two virtual roots:");
			printf("%f+i%f\t\t%f-i%f\n",x1,x2,x1,x2 );
		}
	}
    while (1) {};
}



