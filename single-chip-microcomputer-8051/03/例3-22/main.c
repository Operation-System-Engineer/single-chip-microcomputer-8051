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

	void input(int arr[],int n);/*��������*/
	void sort(int *ptr,int n);
	void output(int arr[],int n);
	int a[N],*p;/*����һά�����ָ�����*/

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
	input(a,N) ;/*�������뺯�����ã�ʵ��a ��������*/
	p = a ;/*ָ�����ָ��������׵�ַ*/
	sort(p,N) ;/*����ʵ��p ��ָ�����*/
	output(p,N) ;/*�����ʵ��p ��ָ�����*/
    while (1) {};
}
void input(int arr[],int n)/*���践��ֵ���������ݺ�������,�β�a r r ������*/
{
	int i;
	printf("input data:\n");
	for( i = 0 ; i < n ; i++ )/*���ô�ͳ���±귨*/
		scanf( "%d" , &arr[i] ) ;
}
void sort(int *ptr,int n)/*ð�������β�p t r ��ָ�����*/
{
	int i,j,t;
	for( i = 0 ; i < n - 1 ; i++ )
		for( j = 0 ; j < n - 1 - i ; j++ )
			if (*(ptr+j)>*(ptr+j+1))/*��������Ԫ�ؽ��бȽ�*/
			{
				t = * (ptr+ j ) ;/*����Ԫ�ؽ��н���*/
				*(ptr+ j ) = * (ptr +j+1) ;
				*(ptr+ j + 1 ) = t ;
			}
}
void output(int arr[],int n)/*�������*/
{
	int *ptr=arr;/*����ָ��ָ��������׵�ַ*/
	printf("output data:\n");
	for( ; ptr-arr<n ; ptr++ )/*��������n ��Ԫ��*/
		printf("%4d",*ptr) ;
	printf("\n");
}



