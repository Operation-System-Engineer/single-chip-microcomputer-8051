/* 外部中断触发读取运行中定时器的值 */

#ifndef __DEMO_4_12_C__
#define __DEMO_4_12_C__

#include <AT89X51.H>

#define _MHZ_ 12    // 设置单片机使用的晶振频率

void main()
{
	/* 定时、中断初始化 */
	TMOD = 0x10;    // T1使用定时模式，工作模式1，无门控位
	TH1 = 0x3C;    // 为T1填入初值，定时时间50ms
	TL1 = 0xB0;
	TR1 = 1;    // 启动T1
	IT0 = 1;    // 设置INTR0中断方式为边沿触发方式，负跳变时产生中断
	ET1 = 1;    // 允许定时器1中断
	EX1 = 1;    // 允许外部中断0中断
	EA = 1;    // CPU开放中断

	while(1);    // 循环等待
}

/* T1溢出中断处理函数 */
void timer1_int() interrupt 3 using 2		// T1溢出中断，使用工作组2
{
	TH1 = 0x3C;    // 重新填入初值
	TL1 = 0xB0;
}

/* 外部中断0处理函数 */
void intr0_int() interrupt 0 using 2		// INTR0中断，使用工作组2
{
	unsigned char tmp1, tmp2;	
	
	do    // 读取T1中的内容
	{
	tmp1 = TH1;
	tmp2 = TL1;
	}while(tmp1 != TH1);    // 数据无效则反复循环
			
	P0 = tmp2;    // 发送有效数据
	P2 = tmp1;
} 

#endif
