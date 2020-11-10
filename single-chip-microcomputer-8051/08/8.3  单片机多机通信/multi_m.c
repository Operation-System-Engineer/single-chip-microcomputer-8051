/* multi_m.c */
/* 多机通信的主机部分 */
#ifndef __MULTI_M_C__
#define __MULTI_M_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // 数据最大长度
#define _MHZ_ 11    // 设置单片机使用的晶振频率(11.0592MHz)

/* 以下为程序协议中使用的握手信号 */
#define __SUCC_ 0x0f    // 数据传送成功
#define __ERR_ 0xf0    // 数据传送错误

void init_serial();    // 串口初始化
void send_data(unsigned char *buf);    // 发送数据
void delay10ms(unsigned int count);    // 延时子程序(10ms)

void main()
{
	char buf[__MAX_LEN_];
	unsigned char i = 0;
	unsigned char tmp;
	unsigned char addr;    // 该字节用于保存要通信的从机地址

	/* 为缓冲区赋初值 */
	P0 = 0xff;
	while(P1 != 0)    // 每隔100ms从P0口读取，若读取到0则表明数据采集结束
	{
		*(buf+i) = P0;
		delay10ms(10);    // 延时100ms
		P0 = 0xff;
		i++;
	}
	*(buf+i) = 0;    // 缓冲区最后一个字节为0表示数据结束
	/* 读要访问的分机地址 */
	P0 = 0xff;
	addr = P0;
	/* 串口初始化 */
	init_serial();    // 初始化串口
	EA = 0;    // 关闭所有中断

	/* 发送地址帧并接收应答信息，如果接收的信号与发送的地址信息不同，则重新发送地址帧 */
	tmp = addr-1;
	while(tmp != addr)
	{
		/* 发送从机地址 */
		TI = 0;
		TB8 = 1;    // 发送地址帧
		SBUF = addr;
		while(!TI);
		TI = 0;
		/* 接收从机应答 */
		RI = 0;
		while(!RI);
		tmp = SBUF;
		RI = 0;
	}
	/* 发送数据并接收校验信息，如果接收的信号为0FH，表示从机接收成功，否则将重新发送该组数据 */
	tmp = __ERR_;
	while(tmp != __SUCC_)
	{
		send_data(buf);    // 发送数据
		RI = 0;
		while(!RI);
		tmp = SBUF;
		RI = 0;
	}
	while(1);    // 程序结束，进入死循环
}

/* 初始化串口 */
void init_serial()
{
	TMOD = 0x20;	//定时器T1使用工作方式2
	TH1 = 250;    // 设置初值
	TL1 = 250;
	TR1 = 1;    // 开始计时
	PCON = 0x80;    // SMOD = 1
	SCON = 0xd0;	//工作方式3，9位数据位，波特率9600bps，允许接收
}

/* 发送数据 */
void send_data(unsigned char *buf)
{
	unsigned char len;    // 保存数据长度
	unsigned char ecc;    // 保存校验字节
	
	len = strlen(buf);    // 计算要发送数据的长度
	ecc = len;    // 开始进行校验字节计算
	/* 发送数据长度 */
	TI = 0;    
	TB8 = 0;    // 发送数据帧
	SBUF = len;    // 发送长度
	while(!TI);
	TI = 0;
	/* 发送数据 */
	for(i=0; i<len; i++)
	{
		ecc = ecc^(*buf);    // 计算校验字节
		TB8 = 0;    // 发送数据帧
		SBUF = *buf;    // 发送数据
		buf++;
		while(!TI);
		TI = 0;
	}
	/* 发送校验字节 */
	TB8 = 0;    // 发送数据帧
	SBUF = ecc;    // 发送校验字节
	while(!TI);
	TI = 0;
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