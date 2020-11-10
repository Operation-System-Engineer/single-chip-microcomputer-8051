#include<REG66x.H>
/* 主器件函数库 */


/*******************************************
            申请总线
功能:进行I2C总线的初始化--包括时钟速率,I2C使能,发送起始信号
********************************************/
void getbus()
{
	S1CON = 0x43;    // 设置时钟为75k(12M),ENS1置位
	STA = 1;    // 申请总线主机,启动总线
	while(SI == 0);    // 等待起始位的发送
}


/*******************************************
            发送数据函数
功能:用于向总线发送数据
********************************************/
void sendbyte(unsigned char c)
{
	S1DAT = c;    // 将要发送的数据装入S1DAT
	S1CON = 0x43;    // 清除SI位
	while(SI == 0);    // 等待数据发送
}

/*******************************************
            向无子地址器件发送字节数据函数
功能:从启动总线到发送地址,数据,结束总线的全过程,从器件地址sla,
待发送的数据c,如如果返回1表示操作成功,否则表示操作有误.
********************************************/
bit isendbyte(unsigned char sla, unsigned char c)
{
	getbus();    // 启动总线
	sendbyte(sla);    // 发送从器件地址,若无应答则返回
	if(S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(c);    // 发送数据
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x43;    // 结束总线
	return 1;
}

/*********************************************************
            向有子地址器件发送多字节数据函数
功能:从启动总线到发送地址,子地址,数据,结束总线的全过程,从器件地址sla
子地址suba,发送内容事s指向的内容,发送no哥字节.如果返回1表示操作成功,
否则操作有误.
**********************************************************/
bit isendstr(unsigned char sla, unsigned char suba,
			 unsigned char *s, unsigned char no)
{
	unsigned char i;

	getbus();    // 启动总线
	sendbyte(sla);    // 发送从器件地址
	if (S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(suba);    // 发送器件子地址
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	for(i=0; i<no; i++)
	{
		sendbyte(*s);    // 发送数据
		if(S1STA != 0x28)
		{
			S1CON = 0x53;
			return 0;
		}
		s++;
	}
	S1CON = 0x53;
	return 1;
}

/*********************************************************
         向无子地址器件读字节数据函数
功能: 从启动总线到发送地址,读数据,借宿总线的全过程,从器件
地址sla,数据值c,如果返回1表示操作成功,否则操作有误.
***********************************************************/
bit irecvbyte(unsigned char sla, unsigned char *c)
{
	gerbus();    // 启动总线
	sendbyte(sla+1);    // 发送器件地址
	if(S1STA != 0x40)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x43;    // 接收一字节数据即发送非应答位
	while(SI == 0);    // 等待接收数据
	if(S1STA != 0x58)
	{
		S1CON=0x53;
		return 0;
	}
	*c = S1DAT;
	S1CON = 0x53;
	return 1;
}

/*********************************************************
        向有子地址器件读取多字节数据函数
功能:从启动总线到发送地址,子地址,读数据,结束总线的全过程,从
器件地址sla,子地址suba,读出内容放入s指向的存储区,读no个字节.
如果返回1表示操作成功,否则操作有误.
**********************************************************/
bit irecvstr(unsigned char sla, unsigned char suba,
			unsigned char *s, unsigned char no)
{
	unsigned char i;
	getbus();    // 启动总线
	sendbyte(sla);    // 发送从器件地址
	if(S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(suba);    // 发送器件子地址
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x63;    // 重新启动总线
	while(SI == 0);
	sendbyte(sla+1);
	if(S1STA != 0x40)
	{
		S1CON = 0x53;
		return 0;
	}
	for(i=0; i<no; i++)
	{
		S1CON = 0x47;    // 接收一个字节数据并发送应答位
		while(SI == 0);    // 等待接收数据
		if(S1STA != 0x50)
		{
			S1CON = 0x53;
			return 0;
		}
		*s=S1DAT;    // 读取数据
		s++;
	}
	S1CON = 0x43;    // 接收最后一字节数据并发送非应答位
	while(SI == 0);
	*s = S1DAT;
	S1CON = 0x53;    // 结束总线
	return 1;
}

/***********************************************************************
                对于时钟芯片pcf8573
 * 从地址格式:        msb    1  1  0  1  0  A1  A0  R/~W
               A1 A0为硬件实现其从地址
 * 主发送帧格式:      SLA+R/~W  A  MODE POINTER  A    DATA    A       P
                                                     n位数据     ||{自动b1,b0增加} 
    MODE POINTER:    0  c2  c1  c0  0 b2  b1  b0
    其含义:     0  c2  c1  c0  
                0   0   0   0     execute address
                0   0   0   1     read control/status flags
                0   0   1   0     reset prescaler, including seconds counter; without carry for minute counter
                0   0   1   1     time adjust, with carry for minute counter (note 1)
                0   1   0   0     reset NODA flag
                0   1   0   1     set NODA flag
                0   1   1   0     reset COMP flag
                
                
                0  b2  b1  b0
                0   0   0   0     time counter hour
                0   0   0   1     time counter minutes
                0   0   1   0     time counter days
                0   0   1   1     time counter months
                0   1   0   0     alarm register hours
                0   1   0   1     alarm register minutes
                0   1   1   0     alarm register days
                0   1   1   1     alarm register months
    只有当c2,c1,c0为0,0,0时,数据才和执行地址相匹配.
                MSB DATA LSB
        ADDRESSED TO: UPPER DIGIT LOWER DIGIT
             UD UC UB UA LD LC LB LA
                0 0 D D D D D D hours
                0 D D D D D D D minutes
                0 0 D D D D D D days
                0 0 0 D D D D D months
                0 0 0 m s NODA COMP POWF control/status flags
*********************************************************************************/


     
    
               
                
                
                
                
                
                
                
