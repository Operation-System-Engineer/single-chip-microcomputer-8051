#pragma SRC
#include <stdio.h>
#include <reg51.h>


#define		TRUE			1
#define		FALSE			0

#define		DELAY_VALUE		3




#define    	PinDrvKey1	P1_0
#define		PinDrvKey2	P1_1
#define		PinDrvKey3	P1_2
#define    	PinDrvKey4	P1_3		//键盘扫描的驱动线


#define    	PinScanKey1	P1_4
#define    	PinScanKey2	P1_5
#define    	PinScanKey3	P1_6
#define    	PinScanKey4	P1_7		//键盘扫描的信号获取线

#define		PinLamp		P0_0		//指示灯驱动管脚
#define		PinSpeaker	P0_1		//蜂鸣器驱动管脚

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
void delay(short i);
unsigned char keyscan(void);

void main(void)
{
	
	unsigned char keyword;
	
	initial();			//键盘驱动信号都设为低
	
	keyword=keyscan();
	
}

void initial(void)
{
	P1_0=FALSE;
	P1_1=FALSE;
	P1_2=FALSE;
	P1_3=FALSE;			//键盘驱动信号都设为低
	P0_0=TRUE;			//发光二极管设为不亮
	P0_1=TRUE;			//蜂鸣器不叫
}

unsigned char keyscan()
{
PinDrvKey1=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE;
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey1==TRUE)			
	return 1;}					//确认确实有按键信号，返回键值
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey2==TRUE)			
	return 5;}					//确认确实有按键信号，返回键值
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey3==TRUE)			
	return 9;}					//确认确实有按键信号，返回键值
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey2==TRUE)
	return 12;}					//确认确实有按键信号，返回键值
PinDrvKey1=FALSE;				//以上语句扫描第一列
PinDrvKey2=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey1==TRUE)
	return 2;}					//确认确实有按键信号，返回键值
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey2==TRUE)
	return 6;}					//确认确实有按键信号，返回键值
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey3==TRUE)
	return 0;}					//确认确实有按键信号，返回键值
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey4==TRUE)
	return 13;}					//确认确实有按键信号，返回键值
PinDrvKey2=FALSE;				//以上语句扫描第二列
PinDrvKey3=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey1==TRUE)
	return 3;}					//确认确实有按键信号，返回键值
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey2==TRUE)
	return 7;}					//确认确实有按键信号，返回键值
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey3==TRUE)
	return 10;}					//确认确实有按键信号，返回键值
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey4==TRUE)
	return 14;}					//确认确实有按键信号，返回键值
PinDrvKey3=FALSE;				//以上语句扫描第三列
PinDrvKey4=TRUE;
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey1==TRUE)
	return 4;}					//确认确实有按键信号，返回键值
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey2==TRUE)
	return 8;}					//确认确实有按键信号，返回键值
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey3==TRUE)
	return 11;}					//确认确实有按键信号，返回键值
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//如果判断有信号，则延时一次
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//假如判断还有信号，则再延时
if(PinScanKey4==TRUE)
	return 15;}					//假如判断还有信号，则再延时
PinDrvKey3=FALSE;				//以上语句扫描第四列
return 16;
}


void delay(short i)
{
	int j=0;
	int k=0;
	k=i*DELAY_VALUE;
	while (j<k)j++;
}
