#include <stdio.h>
#include <reg51.h>

#define TxDATA (unsigned xdata char*)1000H
void initial(void);
unsigned xdata char *p;
int i=0;


main()
{
	initial();
	p=TxDATA;
	ACC=p[0];
	SBUF=ACC;
	while(1);
}

void Rcv_INT(void) interrupt 4
{
	if(TI)
	{
		if(i<0x4F)
		{
			i++;
			ACC=p[i];
			SBUF=ACC;
		}
		TI=0;
	}
}

void initial(void)
{
	IP=0x10;              		//定义串口为高优先级中断
	IE=0x90;              		//允许串口、中断0、1、定时器0
	TCON=0x05;
	TMOD=0x20;            		//定时器1为自动装入(auto-load)方式
	
	PCON=0;               		//SMOD(PCON.7)=1时，baudrate doubled.//smod=1 (11M晶振时为0)
	
	SCON=0xD0;            		//串行口工作方式 : 9位UART, 数据传输率可变
	TH1=0xf3;
	TL1=0xf3;             		//
	PCON=0x80|PCON;	      	//数据传输率设置 : 9600 baud(E8--24MHZ)
	TR1=1;                		//启动定时器1
}

