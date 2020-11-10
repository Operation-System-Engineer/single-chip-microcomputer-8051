/* 运行中读取定时器的值 */

#ifndef __DEMO_4_15_C__
#define __DEMO_4_15_C__

#include <AT89X51.H>

#define _MHZ_ 12    // 设置单片机使用的晶振频率

void delay10ms(unsigned int count);    // 声明延时函数

void main()
{
	bit btmp;
	unsigned char tmp1, tmp2;
	
	/* 定时、中断初始化 */
	TMOD = 0x10;    // T1使用定时模式，工作模式1，无门控位
	TH1 = 0x3C;    // 为T1填入初值，定时时间50ms
	TL1 = 0xB0;
	TR1 = 1;    // 启动T1
	ET1 = 1;    // 允许定时器1中断
	EA = 1;    // CPU开放中断

	while(1)    // 循环检测
	{
		P1_0 = 1;    // 读取P1.0前，应将其寄存器置1
		btmp = P1_0;
		if(btmp)    // 检测P1.0，如果为高电平，则读取定时器值
		{
			do    // 读取T1中的内容
			{
			tmp1 = TH1;
			tmp2 = TL1;
			}while(tmp1 != TH1);    // 数据无效则反复循环
			
			P0 = tmp2;    // 发送有效数据
			P2 = tmp1;
		}
		delay10ms(1);    // 延时10ms
	}
	
}

/* 延时10ms，精度较低，参数count为延时时间 */
void delay10ms(unsigned int count)
{
	unsigned int i, k;
	unsigned char j;
	unsigned int tmp;
	tmp = (int)((100*_MHZ_)/12);

	for(i=0; i<count; i++)
		for(j=0; j<100; j++)
			for(k=0; k<tmp; k++);
}

/* T1溢出中断处理函数 */
void timer1_int () interrupt 3 using 2		// T1溢出中断，使用工作组2
{
	TH1 = 0x3C;    // 重新填入初值
	TL1 = 0xB0;
}

#endif
