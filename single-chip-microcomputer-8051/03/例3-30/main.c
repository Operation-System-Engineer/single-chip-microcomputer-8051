#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <string.h>
#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {
	char *ptr1[4],str[4][20],temp[20];
	/*定义指针数组、二维字符数组、用于交换的一维字符数组*/
	int i,j;

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
		gets(str[i],20);/*输入4个字符串*/
	printf("\n") ;
	for(i=0;i<4;i++)
		ptr1[i] = str[i] ;/*将二维字符数组各行的首地址传递给指针数组的各指针*/
	printf("original string:\n");
	for(i=0;i <4;i++)/*按行输出原始各字符串*/
		printf("%s\n",ptr1[i]) ;
	printf("ordinal string:\n");
	for(i=0;i<3;i++)/*冒泡排序*/
		for ( j = 0 ; j <4-i-1; j++)
			if(strcmp(ptr1[j],ptr1[j+1])>0)
			{
				strcpy(temp,ptr1[j]);
				strcpy(ptr1[j],ptr1[j+1]) ;
				strcpy(ptr1[j+1],temp) ;
			}
	for( i=0;i<4;i++)/*输出排序后的字符串*/
		printf("%s\n" , ptr1[i]);
    while (1) {};
}



