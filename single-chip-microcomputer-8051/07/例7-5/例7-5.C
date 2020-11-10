/* T0使用工作模式3 */

#ifndef __DEMO_4_16_C__
#define __DEMO_4_16_C__

#include <AT89X51.H>

void main()
{
	/* 定时、中断初始化 */
	TMOD = 0x03;    // T0使用定时模式，工作模式3，无门控位
	TL0 = 0x38;    // 为TL0填入初值，定时时间0.2ms
	TH0 = 0x9C;    // 为TH0填入初值，定时时间0.1ms
	TR0 = 1;    // 启动TL0
	TR1 = 1;    // 启动TH0
	ET0 = 1;    // 允许定时器0中断，此时该中断被TL0占用
	ET1 = 1;    // 允许定时器1中断，此时该中断被TH0占用
	EA = 1;    // CPU开放中断

	while(1);    // 循环等待
}

/* T0溢出中断处理函数 */
void timer0_int() interrupt 1 using 2		// T0溢出中断，使用工作组2
{                                           // 此时该中断被TL0使用
	TL0 = 0x38;    // 重新填入初值

	P1_0 = !P1_0;    // 在P1.0口产生方波
}

/* T1溢出中断处理函数 */
void timer1_int() interrupt 3 using 2		// T1溢出中断，使用工作组2
{                                           // 此时该中断被TH0使用
	TH0 = 0x9C;    // 重新填入初值

	P1_1 = !P1_1;    // 在P1.1口产生方波
}

#endif
