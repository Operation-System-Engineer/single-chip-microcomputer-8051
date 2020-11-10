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

	int j,n,p,q,flagp,flagq;
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
	printf("please input n :");
	scanf("%d",&n);
	if (((n%2)!=0)||(n<=4))
		printf("input data error!\n");
	else
	{
		p = 1 ;
		do {
			p = p + 1 ;
			q = n - p ;
			flagp=1;
			for(j=2; j <= (int)(floor(sqrt((double)(p))));j++)
			{
				if ((p%j)==0)
				{
					flagp=0;
					break;
				}
			}
			flagq=1;
			for (j=2;j<=(int)(floor(sqrt((double)(q))));j++)
			{
				if ((q%j)==0)
				{
					flagq= 0 ;
					break;
				}
			}
		} while (flagp*flagq==0);
		printf("%d = %d + %d \n",n,p,q);
	}
    while (1) {};
}

