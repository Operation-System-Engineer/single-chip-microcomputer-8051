#include <REG52.H>                /* special function register declarations   */
                                  /* for the intended 8051 derivative         */

#include <stdio.h>                /* prototype declarations for I/O functions */
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
#define classes 3
#define grades 30




void disp_grades(int g[ ][grades]) /*显示学生成绩*/
{
	int t ,i ;
	for(t=0; t<classes; ++t) {
		printf("class # %d:\n",t + 1 ) ;
		for(i=0;i<grades;++i)
		printf("grade for student #%d is %d\n",i+1,g [t][i] ) ;
	}
}
int get_grades(int num)
{
	char s[80];
	printf("enter grade for student # %d:\n",num+1) ;
	scanf("%s",s);/*输入成绩*/
	return(atoi(s));
}

void enter_grades(int a[][grades])
{
	int t, i;
	for (t=0;t<classes;t++)
	{
		printf (" class #%d:\n",t+1);
		for (i=0; i<grades; i++)
		a[t][i]=get_grades(i);
	}
}

/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
void main (void) {

	int a[classes][grades];/*定义二维数组，每行存放一个班学生成绩 */
	char ch;
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
	for( ; ;)
	{
		do { /*菜单显示*/
			printf("(E)nter grades\n");
			printf("(R)eport grades\n");
			printf("(Q)uit\n");
			ch=toupper(getchar()); /*将键盘输入字符转换大写*/
		} while(ch!='E' && ch!='R' && ch!='Q');
		switch(ch)
		{
			case 'E':
			enter_grades(a);
			break;
			case 'R':
			disp_grades(a);
			break;
			case 'Q':printf("Quit This Program");
			while(1);
		}
	}
    while (1) {};
}



