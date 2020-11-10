#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	int binary(char *ptr[],char *str,int n);/*查找函数声明*/
	void insert(char *ptr[],char *str,int n,int i);/*插入函数声明*/
	char *temp,*ptr1[6];
	int i;
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
	for (i=0;i<5;i++)
	{
		ptr1[i]=malloc(20);/*为指针分配地址后*/
		gets(ptr1[i],20);/*输入字符串*/
	}
	ptr1[5]=malloc(20);
	printf("\n" ) ;
	printf("original string:\n");
	for(i=0;i<5;i++)/*输出指针数组各字符串*/
		printf("%s\n",ptr1[i]) ;
	printf("input search string:\n");
	temp=malloc(20) ;
	gets(temp,20);/*输入被插字符串*/
	i=binary(ptr1,temp,5);/*寻找插入位置i */
	printf("i=%d\n",i) ;
	insert(ptr1,temp,5,i) ;/*在插入位置i 处插入字符串*/
	printf("output strings:\n");
	for(i=0;i<6;i++)/*输出指针数组的全部字符串*/
		printf("%s\n",ptr1[i]) ;
    while (1) {};
}

int binary(char *ptr[],char *str,int n)
{/*折半查找插入位置*/
	int hig,low,mid;
	low = 0 ;
	hig= n - 1 ;
	if (strcmp(str,ptr[0])<0) return 0;
	/*若插入字符串比字符串数组的第0个小，则插入位置为0 */
	if (strcmp(str,ptr[hig])>0) return n;
	/*若插入字符串比字符串数组的最后一个大，则应插入字符串数组的尾部*/
	while(low<=hig)
	{
		mid=(low+hig)/2;
		if (strcmp(str,ptr[mid])<0)
			hig=mid-1;
		else if(strcmp(str,ptr[mid])>0)
			low=mid+1;
		else return(mid);/*插入字符串与字符串数组的某个字符串相同*/
	}
	return low;/*插入的位置在字符串数组中间*/
}
void insert(char *ptr[],char *str,int n,int i)
{
	int j;
	for (j=n;j>i;j--)/*将插入位置之后的字符串后移*/
		strcpy(ptr[j],ptr[j-1]) ;
	strcpy(ptr[i],str);/*将被插字符串按字典顺序插入字符串数组*/
}



