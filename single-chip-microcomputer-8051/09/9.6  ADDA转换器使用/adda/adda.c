//#pragma SRC
#include <stdio.h>
#include <reg51.h>


#define		TRUE			1
#define		FALSE			0

#define		DELAY_VALUE		3

#define		ATODHIGH   0x0700	//00000111
#define		ATODLOW    0x0F00	//00001111
#define		DTOAHIGH   0x1100	//00010001
#define		DTOALOW    0x1200	//00010010
#define		ADDARATE	1.1304
#define		STEPLENGTH	3

#define		PinLamp		P1_0		//指示灯驱动管脚
#define		PinSpeaker	P1_1		//蜂鸣器驱动管脚
#define		PinADStatus	P1_2		//AD的状态显示
#define		PinADCS		P2_4		//AD的选中信号
#define 	XBYTE ((unsigned char volatile xdata*) 0)

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


void initial(void);              //初始化子程序
void delay(unsigned char i);
void sendda(unsigned short ab);
unsigned short getad(void);
unsigned short serdate(void);

void main(void)
{
unsigned short refint=0,aint=0;
initial();			
while(1)
{
refint=serdate();
aint=getad();
if(aint>=refint)
{
	aint-=STEPLENGTH;
	}
else if(aint<refint)
{
	aint+=STEPLENGTH;
	}
sendda(aint);
}
}


void initial(void)
{
P1_0=TRUE;			//发光二极管设为不亮
P1_1=TRUE;			//蜂鸣器不叫
}

void delay(unsigned char i)	//延迟子程序
{
  short j=0;
  short k=0;
  k=i*DELAY_VALUE;
  while (j<k)j++;
}

void sendda(unsigned short ab)
{

XBYTE[DTOAHIGH]=(unsigned char)(ab/256);
delay(DELAY_VALUE);
XBYTE[DTOALOW]=ab-(unsigned short)((unsigned char)(ab/256)*256);
delay(DELAY_VALUE);
}

unsigned short getad(void)
{
unsigned short ab;
unsigned char tmp1,tmp2;	
PinADCS=TRUE;
PinADStatus=TRUE;
while(PinADStatus==TRUE);
tmp1=XBYTE[ATODHIGH];
tmp2=XBYTE[ATODLOW];
ab=(unsigned short)tmp1*256+(unsigned short)tmp2;
return ab;
}

unsigned short serdate(void)
{
unsigned short ab;
unsigned char tmp1,tmp2;
tmp1 = XBYTE[DTOAHIGH];
tmp2 = XBYTE[DTOALOW];
ab=(unsigned short)(((float)tmp1*256+(float)tmp2)*ADDARATE);
return ab;
}
