#include <REG52.H>                /* special function register declarations   */
/* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include<stdlib.h>
#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
/* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/

struct stu
{
	char name[20];
	long number;
	float score[4];
} ;

//void delay();

void main (void) {
	void input(struct stu arr[],int n);/*函数声明*/
	void aver(struct stu arr[],int n);
	void order(struct stu arr[],int n);
	void output(struct stu arr[],int n);
	void out_row(struct stu arr[],int n);
	struct stu stud[4];/*定义结构体数组*/
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
	input(stud,4) ;/*依此调用自定义函数*/
	aver(stud,4) ;
	order(stud,4) ;
	output(stud,4) ;
	out_row(stud,4) ;
    while (1) {};
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
void input(struct stu arr[],int n)
{
	int i,j;
	char temp[30];
	for (i=0;i<n;i++)
	{
		printf("\nInput Name,Number,English,Mathema,Physic\n");
		gets(arr[i].name,20);
		gets(temp,20);
		arr[i].number=atol(temp);
		for(j=0;j<3;j++)
		{
			gets(temp,20);
			arr[i].score[j]=atoi(temp);
		} ;
	}
}
/* * * * * * * * * * * * * * * * * * * * * * */
void aver(struct stu arr[],int n)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		arr[i].score[3]=0;
		for(j=0;j<3;j++)
			arr[i].score[3]=arr[i].score[3]+arr[i].score[j];
		arr[i].score[3]=arr[i].score[3]/3;
	}
}
/* * * * * * * * * * * * * * * * * * * * * * */
void order(struct stu arr[],int n)
{ 
	struct stu temp;
	int i,j;
	for(i=0;i<n-1;i++)
		for(j=0;j<n-1-i;j++)
			if (arr[j].score[3]>arr[j+1].score[3])
			{temp=arr[j];
			arr[j]=arr[j+1];
			arr[j+1]=temp;
			}
}
/* * * * * * * * * * * * * * * * * */
void output(struct stu arr[],int n)
{
	int i,j;
//	printf( " * * * * * * * * * * * * * * * * * TABLE* * * * * * * * * * * * * * * * * * \ n " ) ;
/*	delay();
	printf( " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \ n " ) ;
	delay();
*/	printf("|%10s|%8s|%7s|%7s|%7s|% s|\n","Name","Number","English","mathema","physics","average");
//	printf(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n") ;
	for (i=0;i<n;i++)
	{
		printf("|%10s|%8ld|",arr[i].name,arr[i].number);
		for(j=0;j<4;j++)
			printf("%7.2f|",arr[i].score[j]);
		printf("\n");
//		printf( " - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \ n " ) ;
	}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void out_row(struct stu arr[],int n)
{
	float row[4]={0,0,0,0};
	int i,j;
	for(i=0;i<4;i++)
	{
		for(j=0;j<n;j++)
			row[i]=row[i]+arr[j].score[i];
		row[i]=row[i]/n;
	}
	printf("|%19c|",' ');
	for (i=0;i<4;i++)
		printf("%7.2f|",row[i]) ;
	printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \ n") ;
}


/*void delay()
{
	int i;
	for(i=2000;i>0;i--);
}
*/
