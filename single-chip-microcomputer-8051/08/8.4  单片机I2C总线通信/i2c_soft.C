/* 单主器件I2C总线模拟子程序 */

#include <At89x51.h>
#include <intrins.h>

#define uchar unsigned char
#define DELAY5US _nop_();_nop_();_nop_();_nop_();    // 需要根据晶振频率调整

sbit VSDA=P1^0;    // 将p1.0口模拟数据口
sbit VSCL=P1^1;    // 将p1.1口模拟时钟口

uchar idata SLA;    // 从器件地址
uchar idata SLAW;    // 从器件写地址
uchar idata SLAR;    // 从器件读地址
uchar idata NUMBYT;    // 数据传送字节
uchar idata MTD[10];    // 数据发送缓冲区
uchar idata MRD[10];    // 数据接收缓冲区
bit   bdata NACK;    // 器件坏或错误标志位

/* 启动I2C总线子程序 */
void STA(void)
{ 
	VSDA = 1;    // 启动I2C总线
	VSCL = 1;
	DELAY5US    // 延时4.7us，根据晶振频率调整空操作个数，这里以fosc=12MHz，下同
	VSDA = 0;
	DELAY5US
	VSCL = 0;
}

/* 停止I2C总线数据传送子程序 */
void STOP(void)
{ 
	VSDA = 0;    // 停止I2C总线数据传送
	VSCL = 1;
	DELAY5US
	VSDA = 1;
	DELAY5US
	VSCL = 0;
}

/* 发送应答位子程序 */
void MACK(void)
{
	VSDA = 0;    // 发送应答位
	VSCL = 1;
	DELAY5US
	VSDA = 1;
	VSCL = 0;
}

/* 发送非应答位子程序 */
void MNACK(void)
{ 
	VSDA = 1;    // 发送非应答位
	VSCL = 1;
	DELAY5US
	VSDA = 0;
	VSCL = 0;
}

/* 应答位检查子程序 */
void CACK(void)
{ 
	VSDA = 1;    // 应答位检查（将p1.0设置成输入，必须先向端口写1）
	VSCL = 1;
	F0 = 0;
	if(VSDA == 1)    // 若VSDA=1表明非应答，置位非应答标志F0
		F0 = 1;
	VSCL = 0;
}

/* 发送一个字节数据子程序，程序入口p为发送缓冲区地址 */
void WRBYT(uchar idata *p)
{ 
	uchar idata n=8;     // 向VSDA上发送一位数据字节，共八位
	uchar idata temp;
	temp=*p;
	while(n--)
	{ 
		if((temp&0x80) == 0x80)    // 若要发送的数据最高位为1则发送位1
		{
			VSDA = 1;    // 传送位1
			VSCL = 1;
			DELAY5US
			VSDA = 0;
			VSCL = 0;    
		}
		else
		{  
			VSDA = 0;    // 否则传送位0
			VSCL = 1;
			DELAY5US
			VSCL = 0;  
		}
		temp = temp<<1;    // 数据左移一位，或_crol_(*p,1)
	}
}

/* 接收一字节子程序，入口参数p为接收缓冲区地址 */
void RDBYT(uchar idata *p)
{
	uchar idata n=8;    // 从VSDA线上读取一上数据字节，共八位
	uchar idata temp=0;
	while(n--)
	{
		VSDA = 1;
		VSCL = 1;
		temp = temp<<1;    // 左移一位，或_crol_(temp,1)
		if(VSDA == 1)
			temp = temp|0x01;    // 若接收到的位为1，则数据的最后一位置1
		else 
			temp = temp&0xfe;    // 否则数据的最后一位置0
		VSCL=0;
	}
	*p=temp;
}

/* 发送n位数据子程序 */
void WRNBYT(uchar *sla, uchar n)
{          
	uchar idata *p;
	STA();    // 启动I2C
	WRBYT(sla);    // 发送一上位数据
	CACK();    // 检查应答位
    if(F0 == 1)
	{ 
		NACK = 1;
		return;    // 若非应答表明器件错误或已坏，置错误标志位NACK
	}
	p = MTD;
	while(n--)
	{ 
		WRBYT(p);
		CACK();    // 检查应答位
		if (F0 == 1)
		{
			NACK=1;
			return;    // 若非应答表明器件错误或已坏，置错误标志位NACK
		}
		p++;
	}
	STOP();    // 全部发完则停止
}

/* 接收n位数据子程序 */
void RDNBYT(uchar idata *sla, uchar n)
{ 
	uchar idata *p;
	STA();
	WRBYT(sla);
	CACK();
	if(F0 == 1)
	{
		NACK = 1;
		return;
	}
	p = MRD;
	while(n--)
	{ 
		RDBYT (p);
		MACK();    // 收到一个字节后发送一个应答位
		p++;
	}
	MNACK();    // 收到最后一个字节后发送一个非应答位
	STOP();
}