/* 两级中断嵌套 */

#ifndef __DEMO_4_13_C__
#define __DEMO_4_13_C__
#include <AT89X51.H>

#define _MHZ_ 12    // 设置单片机使用的晶振频率

void delay10ms(unsigned int count);    // 声明延时函数

/* 将得到的秒数转化为时、分、秒的时间格式送显，由于其源码过长，本例未给出 */
void LED_show(unsigned long second);

unsigned char g_count;    // 全局变量，timer_int函数使用
unsigned char g_second;    // 全局变量，timer_int函数使用

void main()
{
	g_count = 0;    // 设置全局变量初值
	g_second = 0;
	
	/* 定时、中断初始化 */
	TMOD = 0x10;    // T1使用定时模式，工作模式1，无门控位
	TH1 = 0xD8;    // 为T1填入初值，定时时间10ms
	TL1 = 0xF0;
	TR1 = 1;    // 启动T1
	IT0 = 1;    // 设置INTR0中断方式为边沿触发方式，负跳变时产生中断
	PT1 = 1;    // 设置定时器1中断为高优先级
	ET1 = 1;    // 允许定时器1中断
	EX1 = 1;    // 允许外部中断0中断
	EA = 1;    // CPU开放中断

	while(1);    // 循环等待
}

/* T1溢出中断处理函数 */
void timer1_int() interrupt 3 using 3		// T1溢出中断，使用工作组3
{                                           // 不同级别的中断函数使用不同的工作组
	TH1 = 0xD8;    // 重新填入初值
	TL1 = 0xF0;

	g_count++;
	if(g_count == 100)    // 计时满1s
	{
		g_count = 0;    // 清0
		g_second++;    // 总秒数增1，溢出时会自动归0
		LED_show(g_second);    // LED送显
	}
}

/* 外部中断0处理函数 */
void intr0_int() interrupt 0 using 2		// INTR0中断，使用工作组2
{
	unsigned char tmp;

	EA = 0;    // 关中断
	P1 = 0xff;    // 在读取P1口数据前，应先对其寄存器置1
	tmp = P1;    // 读P1口
	EA = 1;    // 开中断

	delay10ms(100);

	EA = 0;    // 关中断
	tmp = ~tmp;    // 变量取反
	P1 = tmp;    // 送P1口
	EA = 1;    // 开中断
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

#endif
