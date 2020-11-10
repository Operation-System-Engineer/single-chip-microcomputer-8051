#pragma SRC
#include <stdio.h>
#include <reg51.h>


#define		TRUE			1
#define		FALSE			0

#define		DELAY_VALUE		3
#define		DELAY_VALUE_EXTRA	10100



#define    	PinDrvKey1	P1_0
#define		PinDrvKey2	P1_1
#define		PinDrvKey3	P1_2
#define    	PinDrvKey4	P1_3		
//键盘扫描的驱动线


#define    	PinScanKey1	P1_4
#define    	PinScanKey2	P1_5
#define    	PinScanKey3	P1_6
#define    	PinScanKey4	P1_7		
//键盘扫描的信号获取线

#define		PinSegA		P2_0	
#define		PinSegB		P2_1
#define		PinSegC		P2_2
#define		PinSegD		P2_3		
//七段码的数据线

#define		PinSelectA	P2_4
#define		PinSelectB	P2_5
#define		PinSelectC	P2_6		
//选择器的数据位

#define		PinSelectCS	P2_4		
//选择器的片选位

#define		LedSelect0	0x80
#define		LedSelect1	0x90
#define		LedSelect2	0xa0
#define		LedSelect3	0xb0
#define		LedSelect4	0xc0
#define		LedSelect5	0xd0

#define		PinLamp		P0_0		
//指示灯驱动管脚
#define		PinSpeaker	P0_1		
//蜂鸣器驱动管脚

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


void initial(void);              
//初始化子程序
void delay(short i);		 
//延时子程序
void display(unsigned char i,unsigned char num);
//显示子程序，其中变量i,num含义为在第i个数码管显示数字num.
unsigned char keyscan(void);
//键盘扫描子程序
void updatee(void);

unsigned char h1=0;
unsigned char h0=0;
unsigned char m1=0;
unsigned char m0=0;
unsigned char s1=0;
unsigned char s0=0;
unsigned char seth1=0;
unsigned char seth0=0;
unsigned char setm1=0;
unsigned char setm0=0;
unsigned char sets1=0;
unsigned char sets0=0;  //设定闹钟的闹定时间,设定为闹钟时间，闹铃时间定为1分钟

void main(void)
{
	unsigned char status;
	unsigned char keyword;
	initial();
	//键盘驱动信号，发光二极管，以及蜂鸣器都设在无效状态
	for(;;)
	{
		switch(status)
		{
		case 0:
			keyword=keyscan();
			if(keyword==16)
			{
				if(h1==seth1&&h0==seth0&&m1==setm1&&m0==setm0)
				{
					PinSpeaker=FALSE;
					delay(DELAY_VALUE);
					PinSpeaker=TRUE;     //到达闹钟定时时间，蜂鸣器响
				}
				else
					delay(DELAY_VALUE_EXTRA);
				updatee();
			}
			else
				if(keyword==11)status=1; //0状态为计时状态，1状态为停止状态
				else 
				{
					PinSpeaker=FALSE;
					delay(DELAY_VALUE);
					PinSpeaker=TRUE;     //输入有错，蜂鸣器响
				}
				break;
		case 1:
			keyword=keyscan();
			if(keyword==10)status=0;
			else if(keyword==12)status=2;//2状态为设置初值状态；
			else if(keyword==13){h0=0;
			h1=0;
			s0=0;
			s1=0;
			m0=0;
			m1=0;}//清零
			else if(keyword==14)status=3;//3状态为设置闹钟时间状态；
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //输入有错，蜂鸣器响   			
			}
			break;
		case 2:
			keyword=keyscan();
			if(keyword==10&&s1<=5&&m1<=5&&(h1*10+h0)<24)
				status=0;
			else if(keyword==11&&s1<=5&&m1<=5&&(h1*10+h0)<24)
				status=1;
			else if(keyword<10&&keyword>=0) //输入的是数字键
			{
				h1=h0;
				h0=m1;
				m1=m0;
				m0=s1;
				s1=s0;
				s0=keyword;
				updatee();		   //输入的数字键从低位起往左移
			}
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //输入有错，蜂鸣器响   			
			}
		case 3:
			keyword=keyscan();
			if(keyword==10&&sets1<=5&&setm1<=5&&(seth1*10+seth0)<24)
				status=0;
			else if(keyword==11&&sets1<=5&&setm1<=5&&(seth1*10+seth0)<24)
				status=1;
			else if(keyword<10&&keyword>=0) //输入的是数字键
			{
				seth1=seth0;
				seth0=setm1;
				setm1=setm0;
				setm0=sets1;
				sets1=sets0;
				sets0=keyword;
				updatee();		   //输入的数字键从低位起往左移
			}
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //输入有错，蜂鸣器响   			
			}
			
		}	
	}
	
}

void initial(void)
{
	P1_0=FALSE;
	P1_1=FALSE;
	P1_2=FALSE;
	P1_3=FALSE;			//键盘驱动信号都设为低,避免干扰。  
	P0_0=TRUE;			//发光二极管设为不亮
	P0_1=TRUE;			//蜂鸣器不响
}

void delay(short i)	//延迟子程序，为精确计时，需要用汇编语言来做。R6R7刚好存的是i的值
{
short ii;
ii=i;
#pragma asm
X_delay:
	USING	0
	CLR  	A
	MOV  	R4,A
	MOV  	R5,A
XC0025:
	CLR  	C
	MOV  	A,R5
	SUBB 	A,R7
	MOV  	A,R6
	XRL  	A,#080H
	MOV  	R0,A
	MOV  	A,R4
	XRL  	A,#080H
	SUBB 	A,R0
	JNC  	XC0028
	INC  	R5
	CJNE 	R5,#00H,XC0069
	INC  	R4
XC0069:
	SJMP 	XC0025
XC0028:
#pragma endasm
}

void display(unsigned char i,unsigned char num)
{
	if(num >= 0 && num <=9)
	{switch(i)
	{ case 0:P2= LedSelect0 | num;  
	//LedSelect0的高四位为选择第0个数码管的逻辑，低四位则为数码管显示的数字。
	break;
	  case 1:P2= LedSelect1 | num;
		  //LedSelect1的高四位为选择第1个数码管的逻辑，低四位则为数码管显示的数字。
		  break;
	  case 2:P2= LedSelect2 | num;
		  //LedSelect2的高四位为选择第2个数码管的逻辑，低四位则为数码管显示的数字。
		  break;
	  case 3:P2= LedSelect3 | num;
		  //LedSelect3的高四位为选择第3个数码管的逻辑，低四位则为数码管显示的数字。
		  break;
	  case 4:P2= LedSelect4 | num;
		  //LedSelect4的高四位为选择第4个数码管的逻辑，低四位则为数码管显示的数字。
		  break;
	  case 5:P2= LedSelect5 | num;
		  //LedSelect5的高四位为选择第5个数码管的逻辑，低四位则为数码管显示的数字。
		  break;
	  default:break;
	}
	}
}

unsigned char keyscan()
{
	PinDrvKey1=TRUE;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 1;
	if(PinScanKey2==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 5;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 9;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 12;
	PinDrvKey1=FALSE;	//扫描第一列
	PinDrvKey2=TRUE;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 2;
	if(PinScanKey2==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 6;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 0;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 13;
	PinDrvKey2=FALSE;	//扫描第二列
	PinDrvKey3=TRUE;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 3;
	if(PinScanKey2==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 7;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 10;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 14;
	PinDrvKey3=FALSE;	//扫描第三列
	PinDrvKey4=TRUE;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 4;
	if(PinScanKey2==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 8;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 11;
	if(PinScanKey1==TRUE)
		PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);
	PinLamp=TRUE;
	PinSpeaker=TRUE;
	return 15;
	PinDrvKey3=FALSE;	//扫描第四列
	return 16;
}

void updatee()
{
	if(s0==9)	//如果10ms位需要进位，则进入语句
	{
		s0=0;
		if(s1==5)	//如果100ms位需要进位，则进入语句
		{
			s1=0;
			if(m0==9)	//如果1sec位需要进位，则进入语句
			{
				m0=0;
				if(m1==5)	//如果10sec位需要进位，则进入语句
				{
					m1=0;
					if(h0==9&&h1<=1)
					{h0=0;
					h1+=1;}
					else if(h0==3&&h1==2)
					{h0=0;
					h1=0;}
					else h0+=1;
				}
				else m1+=1;
			}
			else m0+=1;
		}
		else s1+=1;
	}
	else s0+=1;
	display(0,s0);
	delay(DELAY_VALUE);
	display(1,s1);
	delay(DELAY_VALUE);
	display(2,m0);
	delay(DELAY_VALUE);
	display(3,m1);
	delay(DELAY_VALUE);
	display(4,h0);
	delay(DELAY_VALUE);
	display(5,h1);
	delay(DELAY_VALUE); //以上语句则在数码管上显示各个数字
	
}