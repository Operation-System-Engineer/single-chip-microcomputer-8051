/* p2p_m.c */
/* 双机点对点通信的主机部分 */
#ifndef __P2P_M_C__
#define __P2P_M_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // 数据最大长度
#define _MHZ_ 11    // 设置单片机使用的晶振频率(11.0592MHz)

/* 以下为程序协议中使用的握手信号 */
#define __RDY_ 0x06    // 主机开始通信时发送的呼叫信号
#define __BUSY_ 0x15    // 从机忙应答
#define __OK_ 0x00    // 从机准备好
#define __SUCC_ 0x0f    // 数据传送成功
#define __ERR_ 0xf0    // 数据传送错误

void init_serial();    // 串口初始化
void send_data(unsigned char *buf);    // 发送数据
void delay10ms(unsigned int count);    // 延时子程序(10ms)

void main()
{
	char buf[__MAX_LEN_];
	unsigned char i = 0;
	unsigned char tmp = __BUSY_;

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

	/* 串口初始化 */
	init_serial();    // 初始化串口
	EA = 0;    // 关闭所有中断

	/* 发送握手信号06H */
	TI = 0;
	SBUF = __RDY_;
	while(!TI);
	TI = 0;
	/* 接收应答信息，如果接收的信号为00H，表示从机允许接收 */
	while(tmp != __OK_)
	{
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
	SCON = 0x50;	//工作方式1，波特率9600bps，允许接收
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
	SBUF = len;    // 发送长度
	while(!TI);
	TI = 0;
	/* 发送数据 */
	for(i=0; i<len; i++)
	{
		ecc = ecc^(*buf);    // 计算校验字节
		SBUF = *buf;    // 发送数据
		buf++;
		while(!TI);
		TI = 0;
	}
	/* 发送校验字节 */
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