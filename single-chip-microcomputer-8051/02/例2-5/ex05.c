#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <float.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */

#define LOWER 0 /* �������*/
#define UPPER 300 /* �������*/
#define STEP 20 /* ����*/

/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
/* ��ӡ����-�����¶ȶ��ձ�*/
void main (void) {

int fahr;
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

	for ( fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP )
		printf ( "%3d %6.1f\n", fahr, (5.0 / 9.0) * (fahr - 32 ) );

	while (1) {};
}

