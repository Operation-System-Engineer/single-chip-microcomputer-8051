#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <ctype.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */

/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	double sum,atof( char s[ ]);
	char sum1[10],sum2[10];
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
	scanf("%s",sum1);
	scanf("%s",sum2);
	sum=atof(sum1)+atof(sum2);
	printf("%s+%s=%f\n", sum1,sum2,sum);
    while (1) {};
}

double atof( char s[ ])
{
	double val, power;
	int i, sign;
	for ( i = 0; isspace(s[i]); i++ ) /* Ìø¹ý¿Õ°×*/;
		sign = (s[i] == '-' ) ? -1 : 1;
	if ( s[i] == '+' || s[i] == '-' )
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val +(s[i] -'0' );
	if (s[i]  == '.')
		i++;
	for ( power = 1.0; isdigit(s[i]); i++) {
		val = 10.0 * val +(s[i] -'0' );
		power *= 10.0;
	}
	return sign * val / power;
}

