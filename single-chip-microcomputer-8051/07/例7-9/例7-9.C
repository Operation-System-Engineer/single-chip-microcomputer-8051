/********************************************************************* 
 * 程序说明：AT93C56操作函数库，ORG端接地
 * 使用语言：C51
 * 编译工具：Keil uVision2.38a
 * 作者：djws
 * 版本：v1.1
 * 时间：2004.7
 *********************************************************************/

#ifndef __DEMO_4_11_C__
#define __DEMO_4_11_C__

#include <REG51.H>

#define uchar unsigned char
#define uint unsigned int

sbit CS = P1^0;    // 使用sbit命令将P1_0表示为CS
sbit SK = P1^1;    // 使用sbit命令将P1_1表示为SK
sbit DI = P1^2;    // 使用sbit命令将P1_2表示为DI
sbit DO = P1^3;    // 使用sbit命令将P1_3表示为DO

void at93c56_ewen(void);    /* 写使能 */
void at93c56_ewds(void);    /* 写禁止 */
uchar at93c56_read(uchar addr);    /* 读指定地址字节 */
void at93c56_write(uchar addr, uchar mybyte);  /* 将指定字节写入指定存储单元 */
void at93c56_erase(uchar addr);    /* 擦除指定单元 */
void at93c56_wral(uchar mybyte);  /* 将指定字节写入所有存储单元 */
void at93c56_eral(void);    /* 擦除所有存储单元 */

/* 写使能函数，使EEPROM处于可写状态 */
void at93c56_ewen(void)
{
	uchar i, tmp;
	
	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	
	tmp = 0x30;    // 0B0011XXXX
	for(i=0; i<8; i++)    // 送命令字
	{
		DI = tmp&0x80;
		SK = 1; SK = 0;
		tmp <<= 1;
	}
	
	CS = 0;
}

/* 写禁止函数，禁止写入EEPROM */
void at93c56_ewds(void)
{
	uchar i, tmp;
	
	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	
	tmp = 0x00;    // 0B0000XXXX
	for(i=0; i<8; i++)    // 送命令字
	{
		DI = tmp&0x80;
		SK = 1; SK = 0;
		tmp <<= 1;
	}
	
	CS = 0;
}

/* 读指定地址字节，参数addr为内存地址 */
uchar at93c56_read(uchar addr)
{
	uchar i, result;

	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	DI = 1;	SK = 1;	SK = 0;    // 送操作码10
	DI = 0;	SK = 1;	SK = 0;
	
	for(i=0; i<8; i++)    // 送地址
	{
		DI = addr&0x80;
		SK = 1; SK = 0;
		addr <<= 1;
	}
	
	CS = 0;	DO = 1;	CS = 1;    // 置接收端为1
	SK = 0;
	while(!DO)    // 检查DO是否为0，DO为0表示芯片开始传送数据
	{
		SK = 1; SK = 0;
	}
	
	SK = 1;	SK = 0;    // 空过第1位标志位
	result = 0;
	for(i=0; i<8; i++)    // 接收数据
	{
		result <<= 1;
		result = result|DO;
		SK = 1; SK = 0;
	}
	
	CS = 0;

	return(result);
}

/* 将指定字节写入指定存储单元，参数addr为内存地址，参数mybyte为写入数据 */
void at93c56_write(uchar addr, uchar mybyte)
{
	uchar i;
	
	at93c56_ewen();    // 写使能

	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	DI = 0;	SK = 1;	SK = 0;    // 送操作码01
	DI = 1;	SK = 1;	SK = 0;
	
	for(i=0; i<8; i++)    // 送地址
	{
		DI = addr&0x80;
		SK = 1; SK = 0;
		addr <<= 1;
	}
	
	for(i=0; i<8; i++)    // 送数据
	{
		DI = mybyte&0x80;
		SK = 1; SK = 0;
		mybyte <<= 1;
	}
	
	CS = 0;	DO = 1;	CS = 1;    // 置接收端为1
	SK = 0;
	while(DO)    // DO为0表示芯片忙
	{
		SK = 1; SK = 0;
	}
	
	SK = 0; CS = 0;

	at93c56_ewen();    // 写禁止
}

/* 擦除指定单元，参数addr为内存地址 */
void at93c56_erase(uchar addr)
{
	uchar i;
	
	at93c56_ewen();    // 写使能

	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	DI = 1;	SK = 1;	SK = 0;    // 送操作码11
	DI = 1;	SK = 1;	SK = 0;
	
	for(i=0; i<8; i++)    // 送地址
	{
		DI = addr&0x80;
		SK = 1; SK = 0;
		addr <<= 1;
	}
	
	CS = 0;	DO = 1;	CS = 1;    // 置接收端为1
	SK = 0;
	while(DO)    // DO为0表示芯片忙
	{
		SK = 1; SK = 0;
	}
	
	SK = 0; CS = 0;

	at93c56_ewen();    // 写禁止
}

/* 将指定字节写入所有存储单元，参数mybyte为写入数据 */
void at93c56_wral(uchar mybyte)
{
	uchar i, tmp;

	at93c56_ewen();    // 写使能

	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	
	tmp = 0x10;    // 0B0001XXXX
	for(i=0; i<8; i++)    // 送命令字
	{
		DI = tmp&0x80;
		SK = 1; SK = 0;
		tmp <<= 1;
	}
		
	for(i=0; i<8; i++)    // 送数据
	{
		DI = mybyte&0x80;
		SK = 1; SK = 0;
		mybyte <<= 1;
	}
	
	CS = 0;	DO = 1;	CS = 1;    // 置接收端为1
	SK = 0;
	while(DO)    // DO为0表示芯片忙
	{
		SK = 1; SK = 0;
	}
	
	SK = 0; CS = 0;

	at93c56_ewen();    // 写禁止
}

/* 擦除所有存储单元 */
void at93c56_eral(void)
{
	uchar i, tmp;
	
	at93c56_ewen();    // 写使能

	CS = 0;	SK = 0;	CS = 1;    // 时序同步
	DI = 1;	SK = 1;	SK = 0;    // 送起始位1
	
	tmp = 0;    // 0B0000XXXX
	for(i=0; i<8; i++)    // 送命令字
	{
		DI = tmp&0x80;
		SK = 1; SK = 0;
		tmp <<= 1;
	}
	
	CS = 0;	DO = 1;	CS = 1;    // 置接收端为1
	SK = 0;
	while(DO)    // DO为0表示芯片忙
	{
		SK = 1; SK = 0;
	}
	
	SK = 0; CS = 0;

	at93c56_ewen();    // 写禁止
}

#endif
