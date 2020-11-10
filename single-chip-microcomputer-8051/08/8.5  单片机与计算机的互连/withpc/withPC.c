#pragma SRC
#include <stdio.h>
#include <reg51.h>
#define		TRUE			1
#define		FALSE			0

#define		LampComm	P0_0
//以上语句定义主机正在通讯闪灯信号
#define		SelectComm1	P0_1
#define		SelectComm2	P0_2
#define		SelectComm3	P0_3
#define		SelectComm4	P0_4
#define		SelectComm5	P0_5
#define		SelectComm6	P0_6
//以上语句定义主机通讯的从机号
#define 	MACHINECOMM_OK 	 0x11  
//以上语句定义主机通讯传送握手信号
#define 	MACHINECOMM_RET 0x88    
//以上语句定义从机回复通讯握手信号
#define MACHINECOMM_DATAHEAD 	0x5a 
//以上语句定义数据包的头
#define MACHINECOMM_DATAEND 	0xa5 
//以上语句定义数据包的尾

#define		DELAY_VALUE	3

sbit		P0_0=P0^0;
sbit		P0_1=P0^1;
sbit		P0_2=P0^2;
sbit		P0_3=P0^3;
sbit		P0_4=P0^4;
sbit		P0_5=P0^5;
sbit		P0_6=P0^6;
sbit		P0_7=P0^7;

sbit		P1_0=P1^0;
sbit		P1_1=P1^1;
sbit		P1_2=P1^2;
sbit		P1_3=P1^3;
sbit		P1_4=P1^4;
sbit		P1_5=P1^5;
sbit		P1_6=P1^6;
sbit		P1_7=P1^7;

sbit		P2_0=P2^0;
sbit		P2_1=P2^1;
sbit		P2_2=P2^2;
sbit		P2_3=P2^3;
sbit		P2_4=P2^4;
sbit		P2_5=P2^5;
sbit		P2_6=P2^6;
sbit		P2_7=P2^7;

sbit		P3_2=P3^2;
sbit		P3_3=P3^3;


bit EvRcv=FALSE;			//是否收到串行数据的标志位

unsigned char rcvdata=0;		//从串口存储接收的变量
void send(char *temp,int j);		//发送子程序
void initial(void);     		//初始化子程序
void delay(short i);			//延时子程序

void main(void)
{
	unsigned char datacomm[6],datarecv[6];
	unsigned char count=0,datahand;
	datahand=MACHINECOMM_RET;
	datacomm[0]=MACHINECOMM_DATAHEAD;
	datacomm[5]=MACHINECOMM_DATAEND;
	datacomm[1]='w';
	datacomm[2]='o';
	datacomm[3]='r';
	datacomm[4]='k';
	initial();
	SelectComm6=1;//选择与PC机进行通信。
	while(1)
	{
		if(EvRcv)//如果接收到主机端发送过来的握手信号
		{
			if(rcvdata==MACHINECOMM_OK)
			{   LampComm=TRUE;
			delay(DELAY_VALUE);
			LampComm=FALSE;
			}	//指示灯闪烁		
			EvRcv=FALSE;
			break;//退出等待
		}
		send(&datahand,1);//返回一个握手信号
		break;
	}
	for(;;){while(1)
	{if(EvRcv){
		datarecv[count]=rcvdata;
		count++;
		EvRcv=FALSE;
	}
	if(count==5)break;//如果已经收到六个数据，则认为是收到一个数据包
	}//接收一个数据包
	if(datarecv[0]==MACHINECOMM_DATAHEAD&&datarecv[5]==MACHINECOMM_DATAEND)
		send(datacomm,6);//如果数据包正确，则返回一个数据包；
	}
}

void initial()		//初始化子程序
{
	IP=0x10;              //定义串口为高优先级中断
	IE=0x97;              //允许串口、中断0、1、定时器0
	TCON=0x05;
	
	TMOD=0x21;            //定时器1为自动装入(auto-load)方式
	PCON=0;               //SMOD(PCON.7)=1时，波特率翻倍.//smod=1 (晶振为11.0592M时为0)
	
	SCON=0xD0;            //串行口工作方式 : 9位UART, 波特率可变
	TH1=0xf3;
	TL1=0xf3;             //
	PCON=0x80|PCON;	//SMOD=1;		//波特率设置 : 9600 baud(E8--24MHZ)
	//SMOD=0=4.8kbps
	//th1=tl1=fd,smod=0 ,=19.2kbps
	TR1=1;                //启动定时器1
	SelectComm1=0;
	SelectComm2=0;
	SelectComm3=0;
	SelectComm4=0;
	SelectComm5=0;
	SelectComm6=0;
}

void send(unsigned char *temp,int j)
{
	int i=0;
	LampComm=TRUE;
	EA=0;
	for(i=0;i<=j-1;i++)	//按发送数据的长度来发送数据
	{
		ACC=*(temp+i);
		TB8=P;
		SBUF=ACC;	//发送数据
		while(TI==0);	//发送数据完毕时，TI会自动置高
		TI=0;		//发送数据完毕，将TI清零，准备下一次发送
	}
	EA=1;
	LampComm=FALSE;
}

void Rcv_INT(void) interrupt 1
//串口中断优先级为1
{
	LampComm=TRUE;
	if(RI)		//如果收到数据则进行下面的操作
	{
		ACC=SBUF;		//将串行通信的缓存中的数据存入寄存器A
		if(P==RB8)
		{
			rcvdata=ACC;	//将寄存器A中的数据存入变量rcvdata
			EvRcv=TRUE;	//将已经接收到数据的标志位置高
		}
		RI=0;		//RI清零
	}
	LampComm=FALSE;
}

//延时子程序
void delay(int i)
{
	int j=0;
	i=i*DELAY_VALUE;
	while (j<i)j++;
}