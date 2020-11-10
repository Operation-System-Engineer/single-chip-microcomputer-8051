/* multi_s.c */
/* 多机通信的从机部分 */
#ifndef __MULTI_S_C__
#define __MULTI_S_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // 数据最大长度
#define _MHZ_ 11    // 设置单片机使用的晶振频率(11.0592MHz)

/* 以下为程序协议中使用的握手信号 */
#define __SUCC_ 0x0f    // 数据传送成功
#define __ERR_ 0xf0    // 数据传送错误

void init_serial();    // 串口初始化
unsigned char recv_data(unsigned char *buf);    // 接收数据
void Beep_ok();    // 蜂鸣表示数据接收ok，该函数代码未给出

void main()
{
	char buf[__MAX_LEN_];
	unsigned char i = 0;
	unsigned char tmp = 0xff;
	unsigned char addr;    // 保存本机地址

	/* 从P1口读取本机地址 */
	P1 = 0xff;
	addr = P1;
	
	/* 串口初始化 */
	init_serial();    // 初始化串口
	EA = 0;    // 关闭所有中断

	/* 进入设备应答阶段 */
	while(1)
	{
		SM2 = 1;    // 只接收地址帧
		/* 如果接收到的地址帧不是本机地址，则继续等待 */
		tmp = addr-1;
		while(tmp != addr)
		{
			RI = 0;
			while(!RI);
			tmp = SBUF;
			RI = 0;
		}
		/* 发送应答信号，并做好接收数据的准备 */
		TI = 0;
		TB8 = 0;    // 主机不检测该位
		SBUF = addr;
		while(!TI);
		TI = 0;
		SM2 = 0;    // 允许接收数据信息
		/* 数据接收 */
		tmp = 0xff;
		while(tmp == 0xff)    // 如果数据校验失败则重新接收数据
		{
			tmp = recv_data(buf);    // 校验失败返回0xff，检测到地址帧则返回0xfe，接收成功则返回0
		}
		if(tmp == 0xfe)    // 在数据接收过程中，如果发现地址帧，则重新开始整个接收过程
			continue;
		Beep_ok();    // 蜂鸣表示数据接收成功	
	}
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

/* 接收数据，注意该函数使用buf指向的缓冲区保存数据，在数据末尾使用'\0'表示数据结束
 * 返回值为0，数据校验成功，返回值为0xfe，接受过程中接收到地址帧，返回值为0xff，数据校验失败  
 */
unsigned char recv_data(unsigned char *buf)
{
	unsigned char len;    // 该字节用于保存数据长度
	unsigned char ecc;    // 该字节用于保存校验字节
	unsigned char i,tmp;
	/* 接收数据长度 */
	RI = 0;
	while(!RI);
	if(RB8 == 1)    // 若当前接收为地址帧则返回0xfe
		return 0xfe;
	len = SBUF;
	RI = 0;
	/* 使用len的值为校验字节ecc赋初值 */
	ecc = len;
	/* 接收数据 */
	for(i=0; i<len; i++)
	{
		while(!RI);
		if(RB8 == 1)    // 若当前接收为地址帧则返回0xfe
			return 0xfe;		
		*buf = SBUF;    // 接收数据
		ecc = ecc^(*buf);    // 进行字节校验
		RI = 0;
		buf++;
	}
	*buf = 0;    // 表示数据结束
	/* 接收校验字节 */
	while(!RI);
	if(RB8 == 1)    // 若当前接收为地址帧则返回0xfe
		return 0xfe;
	tmp = SBUF;
	RI = 0;
	/* 进行数据校验 */
	ecc = tmp^ecc;
	if(ecc != 0)    // 校验失败
	{
		*(buf-len) = 0;    // 清空数据缓冲区
		TI = 0;    // 发送校验失败信号
		TB8 = 0;
		SBUF = __ERR_;
		while(!TI);
		TI = 0;
		return 0xff;    // 返回0xff表示校验错误
	}
	TI = 0;    // 校验成功
	TB8 = 0;
	SBUF = __SUCC_;
	while(!TI);
	TI = 0;
	return 0;    // 校验成功，返回0
}

#endif