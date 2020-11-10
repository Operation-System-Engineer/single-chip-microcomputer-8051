#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
struct data1/*定义结构体类型*/
{
	int day,month,year;
} ;
struct stu/*定义结构体类型*/
{
	char name[20];
	long num;
	struct data1 birthday;
} ;
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	int i;
	struct stu *p,student[4]={{"liying",1,1978,5,23},{"wangping",2,1979,3,14},
	{"libo",3,1980,5,6},{"xuyan",4,1980,4,21}};
	/*定义结构体数组并初始化*/
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
	p=student;/*将数组的首地址赋值给指针p,p指向了一维数组student*/
	printf("\n1----Output name,number,year,month,day\n" );
	for(i=0;i<4;i++)/*采用指针法输出数组元素的各成员*/
		printf("%20s%10ld%10d//%d//%d\n",(p+i)->name,(p+i)->num,(p+i)->birthday.year,(p+i)->birthday.month,(p+i)->birthday.day);
	printf("\n2----Output name,number,year,month,day\n" );
	for(i=0;i<4;i++,p++)/*采用指针法输出数组元素的各成员*/
		printf("%20s%10ld%10d//%d//%d\n",p->name,p->num,
		p->birthday.year,p->birthday.month,
		p->birthday.day);
	printf("\n3-----Output name,number,year,month,day\n" );
	for(i=0;i<4;i++)/*采用地址法输出数组元素的各成员*/
		printf("%20s%10ld%10d//%d//%d\n",(student+i)->name,(student+i)->num,
		(student+i)->birthday.year,(student+i)->birthday.month,
		(student+i)->birthday.day);
	p=student;
	printf("\n4-----Output name,number,year,month,day\n" );
	for(i=0;i<4;i++)/*采用指针的数组描述法输出数组元素的各成员*/
		printf("%20s%10ld%10d//%d//%d\n",p[i].name,p[i].num,
		p[i].birthday.year,p[i].birthday.month,
		p[i].birthday.day);
    while (1) {};
}



