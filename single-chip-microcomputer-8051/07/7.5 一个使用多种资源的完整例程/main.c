
#ifdef MONITOR51                         /* Debugging with Monitor-51 needs   */
char code reserve [3] _at_ 0x23;         /* space for serial interrupt if     */
#endif                                   /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
/*------------------------------------------------
The main C function.  Program execution starts
here after stack initialization.
------------------------------------------------*/
#include <stdio.h>
#include <reg51.h>
#include <math.h>

#define		DEBUG
#define		VALID			0
#define		INVALID			1
#define		TRUE			1
#define		FALSE			0
#define		ConstClockWise		1
#define		ConstAntiClock		0

#define		DELAY_VALVE		3

#define		MAXSTEPS		400			//maxsteps = 360/0.9=400
#define		HALFMAXSTEPS		200			//180 degree
#define		ONEQUARTERSTEPS		100
#define		THREEQUARTERSTEPS	300
#define		OCTANT			50

#define		MAXINPULSE		20


//#define    	PinZFeedBack	P0_0		//input,photoelectricity signal feed back ,to know where is the 0 angle position
#define		PinClockWiseRot	P0_1		//input,ClockWise Rotate Signal
#define		PinAntiClockRot	P0_2		//input,Anti-clock-wise rotate signal
#define    	PinInFree	P0_3		//input,Rotate to Zero Angle Signal
#define		PinDrvOPTO	P1_0		//output,opto.
#define		PinDrvFree	P1_1		//output,if ,then machine is free
#define    	PinDrvAntiClock	P1_2		//output,if VALID,then Drive the machine rotate ANTI-clock wise
#define    	PinDriver	P1_3		//output,give Pulse to control the machine's speed

#define    	PinXClockWise	P2_0		//input,X-array machine rotate clockwisely
#define    	PinYClockWise	P2_1		//input,Y-array machine rotate clockwisely


#define		PinLamp1	P1_4
#define		PinLamp2	P1_5
#define		PinLamp3	P1_6
#define		PinLamp4	P1_7

#define		PinLamp5	P2_2
#define		PinLamp6	P2_3

#define		PinLampBlink	PinLamp1
#define		PinRotIndicator	PinLamp2
#define		LampFatalError	PinLamp4
#define		PinLampDebug	PinLamp3
#define		LampOVErr	PinLamp5
#define		LampComm	PinLamp6


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


bit EvTimer=FALSE;
bit EvRcv=FALSE;

char rcvdata=0;
char workmode=2;
int delaycount=20;

long int XPulse=0,YPulse=0;
long int XPulseTemp=0,YPulseTemp=0;

//用最小步数表示角度

//int debugreg[4];
int debugYinput=0;
int debugXinput=0;
int counter10ms=0;
int debugtime=0;


void initial(void);              //初始化子程序
void delay(int i);
int FuncRotate(bit BClockWise,int step,int *PRegAngle);
int FuncGoAngleZ(int *PRegAngle);
void send(char *temp,int j);


void main(void)
{
	int RegAngle=0,RegNewAngle=0,temp=0;
	long int Xtmp=0,Ytmp=0;
	
	initial();                                 //初始化子程序
	PinDriver=VALID;
	PinLamp1=FALSE;
	PinLamp2=FALSE;
	PinLamp3=FALSE;
	PinLamp4=FALSE;
	PinLamp5=FALSE;
	PinLamp6=FALSE;
	
	PinDrvOPTO=FALSE;
	PinDrvFree=FALSE;
	delay(1);
	
	while(1)
	{
		while(PinInFree==VALID) 
		{
			TR0=0;
			PinDrvFree=TRUE;
			while(PinClockWiseRot==VALID) 
			{
				PinDrvFree=FALSE;
				FuncRotate(ConstClockWise,1,&RegAngle);
				delay(500);
			}
			while(PinAntiClockRot==VALID)
			{
				PinDrvFree=FALSE;
				FuncRotate(ConstAntiClock,1,&RegAngle);
				delay(500);
			}
			TR0=1;
			EvTimer=FALSE;
		}
		PinDrvFree=FALSE;
		if(EvRcv)
		{
			delaycount=rcvdata&0x1F;
			workmode=(rcvdata&0xE0)/32;
			EvRcv=FALSE;
		}
		
		
		if (EvTimer)
		{
			++debugtime;
			if(debugtime==5) 
			{
				PinLampBlink=TRUE;
			}
			else if(debugtime==10)
			{
				PinLampBlink=FALSE;
				debugtime=0;
			}
			
			RegNewAngle=XPulse*XPulse+YPulse*YPulse;
			RegNewAngle=(int)sqrt((float)RegNewAngle);
			
			temp=RegNewAngle-RegAngle;
			if(temp>HALFMAXSTEPS)
			{
				temp=temp-MAXSTEPS;
			}
			else if(temp<-HALFMAXSTEPS)
			{
				temp=temp+MAXSTEPS;
			}
			
			if(temp>0)
			{
				FuncRotate(ConstClockWise,temp,&RegAngle);
			}
			if(temp<0)
			{
				FuncRotate(ConstAntiClock,-temp,&RegAngle);}
		}
		
		if(workmode==1) 
		{
			send((char*)&RegAngle,2);
		}
		else if(workmode==2) 
		{
			send((char*)&RegAngle,2);
			send((char*)&XPulse,4);
			send((char*)&YPulse,4);
		}
		
		EvTimer=FALSE;
	}
}



void initial(void)
{
	
	//  IP=0x08;              //定义T1中断优先
	//  IE=0x87;              //允许中断0、1、定时器0
	//  TCON=0x05;            //外部中断为负边沿中断
	
	IP=0x10;              //定义串口为高优先级中断
	IE=0x97;              //允许串口、中断0、1、定时器0
	TCON=0x05;
	
	//  TMOD=0x01;            //定时器0以十六位定时/计数器方式工作
	TMOD=0x21;            //定时器1为自动装入(auto-load)方式
	//  TMOD=0x22;            //定时器1为自动装入(auto-load)方式
	//  TH0=0xD8;             //10ms产生中断
	//  TL0=0xEF;
	TH0=0xB1;             //10ms产生中断 24Mhz
	TL0=0xE0;
	//  TH0=0x4E;             //10ms产生中断 24Mhz
	//  TL0=0x20;
	//  TH0=0xFF;             //10ms产生中断 24Mhz
	//  TL0=0x80;
	//  TH0=0xff;             //10ms产生中断 24Mhz
	//  TL0=0x3f;
	//  TH0=0x00;
	//  TL0=0x00;             //
	TR0=1;
	
	
	PCON=0;               //SMOD(PCON.7)=1时，baudrate doubled.//smod=1 (11M晶振时为0)
	
	SCON=0xD0;            //串行口工作方式 : 9位UART, 波特率可变
	TH1=0xf3;
	TL1=0xf3;             //
	PCON=0x80|PCON;	//  SMOD=1;		//波特率设置 : 9600 baud(E8--24MHZ)
	//SMOD=0=4.8kbps
	//th1=tl1=fd,smod=0 ,=19.2kbps
	TR1=1;                //启动定时器1
	
	
	XPulse=0;
	YPulse=0;
	XPulseTemp=0;
	YPulseTemp=0;
	
}

void send(char *temp,int j)
{
	int i=0;
	LampComm=TRUE;
	EA=0;
	for(i=0;i<=j-1;i++)
	{
		ACC=*(temp+i);
		TB8=P;
		SBUF=ACC;
		while(TI==0);
		TI=0;
	}
	EA=1;
	LampComm=FALSE;
}

int FuncRotate(bit BClockWise,int step,int *PRegAngle)
{
	
	PinRotIndicator=TRUE;
	if (BClockWise)
    {
		PinDrvAntiClock=INVALID;
		*PRegAngle+=step;
		if (*PRegAngle>=MAXSTEPS) *PRegAngle-=MAXSTEPS;
    }
	else
    {
		PinDrvAntiClock=VALID;
		*PRegAngle-=step;
		if (*PRegAngle<0) *PRegAngle+=MAXSTEPS;
    }
	
	
	while(step)
    {
		PinDriver=INVALID;
		delay(delaycount);
		PinDriver=VALID;
		delay(delaycount);
		--step;
    }
	PinRotIndicator=FALSE;
	return(TRUE);
	
	
}
void delay(int i)
{
	int j=0;
	i=i*DELAY_VALVE;
	
	while (j<i)j++;
	
}

void XPulse_INT(void) interrupt 2
{
	EX0=0;
	
	if(PinXClockWise)   XPulseTemp++;
	else XPulseTemp--;
	
	EX0=1;
}
void  YPulse_INT(void)  interrupt 0
{
	EX1=0;
	
	if(PinYClockWise) YPulseTemp++;
	else YPulseTemp--;
	
	EX1=1;
}

void  TIMER(void)  interrupt 1
{
	TH0=0xB1;             //10ms产生中断 24Mhz
	TL0=0xE0;
	
	counter10ms++;
	//  if(counter10ms==5)
	//    PinLampBlink=TRUE;
	
	if(counter10ms>=30)
    {
		counter10ms=0;
		if(EvTimer==TRUE){while(1){LampFatalError=TRUE;}}//for debug use
		EvTimer=TRUE;
		//	 PinLampBlink=FALSE;
		XPulse=XPulseTemp;
		YPulse=YPulseTemp;
		XPulseTemp=0;
		YPulseTemp=0;
    }
}



void Rcv_INT(void) interrupt 4
{
	LampComm=TRUE;
	if(RI)
	{
		ACC=SBUF;
		if(P==RB8)
		{
			rcvdata=ACC;
			EvRcv=TRUE;
		}
		
		RI=0;
		
	}
	LampComm=FALSE;
}

