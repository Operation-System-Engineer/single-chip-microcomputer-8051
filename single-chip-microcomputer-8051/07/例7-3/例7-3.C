/* 使用定时器在P1.0口产生200Hz方波 */

#ifndef __DEMO_4_14_C__
#define __DEMO_4_14_C__
#include <AT89X51.H>

void main()
{
	P1 = 0;    // 清P0口
	
	/* 定时、中断初始化 */
	TMOD = 0x0;    // T0使用定时模式，工作模式0，无门控位
	TH0 = 0x60;    // 为T0填入初值，定时时间5ms
	TL0 = 0x78;
	TR0 = 1;    // 启动T0
	ET0 = 1;    // 允许定时器0中断
	EA = 1;    // CPU开放中断

	while(1);    // 循环等待
}

/* T0溢出中断处理函数 */
void timer0_int () interrupt 1 using 2		// T0溢出中断，使用工作组2
{
	TH0 = 0x60;    // 重新填入初值
	TL0 = 0x78;
	P1 ^= 0x01;    // P1.0取反，产生方波
}

#endif
