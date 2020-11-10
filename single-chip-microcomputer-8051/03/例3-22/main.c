#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
#define N 10

/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	void input(int arr[],int n);/*函数声明*/
	void sort(int *ptr,int n);
	void output(int arr[],int n);
	int a[N],*p;/*定义一维数组和指针变量*/

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
	input(a,N) ;/*数据输入函数调用，实参a 是数组名*/
	p = a ;/*指针变量指向数组的首地址*/
	sort(p,N) ;/*排序，实参p 是指针变量*/
	output(p,N) ;/*输出，实参p 是指针变量*/
    while (1) {};
}
void input(int arr[],int n)/*无需返回值的输入数据函数定义,形参a r r 是数组*/
{
	int i;
	printf("input data:\n");
	for( i = 0 ; i < n ; i++ )/*采用传统的下标法*/
		scanf( "%d" , &arr[i] ) ;
}
void sort(int *ptr,int n)/*冒泡排序，形参p t r 是指针变量*/
{
	int i,j,t;
	for( i = 0 ; i < n - 1 ; i++ )
		for( j = 0 ; j < n - 1 - i ; j++ )
			if (*(ptr+j)>*(ptr+j+1))/*相临两个元素进行比较*/
			{
				t = * (ptr+ j ) ;/*两个元素进行交换*/
				*(ptr+ j ) = * (ptr +j+1) ;
				*(ptr+ j + 1 ) = t ;
			}
}
void output(int arr[],int n)/*数据输出*/
{
	int *ptr=arr;/*利用指针指向数组的首地址*/
	printf("output data:\n");
	for( ; ptr-arr<n ; ptr++ )/*输出数组的n 个元素*/
		printf("%4d",*ptr) ;
	printf("\n");
}



