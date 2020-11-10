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

	int x[10]; /* 定义包含1 0 个整型数的数组，引用为x [ 0 ]，x [ 1 ] . . . x [ 9 ] */
	int t ;
	for (t=0; t<10;++t) x[t]=t;
    while (1) {};
}

