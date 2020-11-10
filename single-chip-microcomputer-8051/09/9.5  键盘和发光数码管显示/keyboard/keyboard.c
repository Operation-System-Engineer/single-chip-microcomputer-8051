#pragma SRC
#include <stdio.h>
#include <reg51.h>


#define		TRUE			1
#define		FALSE			0

#define		DELAY_VALUE		3




#define    	PinDrvKey1	P1_0
#define		PinDrvKey2	P1_1
#define		PinDrvKey3	P1_2
#define    	PinDrvKey4	P1_3		//����ɨ���������


#define    	PinScanKey1	P1_4
#define    	PinScanKey2	P1_5
#define    	PinScanKey3	P1_6
#define    	PinScanKey4	P1_7		//����ɨ����źŻ�ȡ��

#define		PinLamp		P0_0		//ָʾ�������ܽ�
#define		PinSpeaker	P0_1		//�����������ܽ�

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


void initial(void);              //��ʼ���ӳ���
void delay(short i);
unsigned char keyscan(void);

void main(void)
{
	
	unsigned char keyword;
	
	initial();			//���������źŶ���Ϊ��
	
	keyword=keyscan();
	
}

void initial(void)
{
	P1_0=FALSE;
	P1_1=FALSE;
	P1_2=FALSE;
	P1_3=FALSE;			//���������źŶ���Ϊ��
	P0_0=TRUE;			//�����������Ϊ����
	P0_1=TRUE;			//����������
}

unsigned char keyscan()
{
PinDrvKey1=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE;
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey1==TRUE)			
	return 1;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey2==TRUE)			
	return 5;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey3==TRUE)			
	return 9;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey2==TRUE)
	return 12;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
PinDrvKey1=FALSE;				//�������ɨ���һ��
PinDrvKey2=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey1==TRUE)
	return 2;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey2==TRUE)
	return 6;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey3==TRUE)
	return 0;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey4==TRUE)
	return 13;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
PinDrvKey2=FALSE;				//�������ɨ��ڶ���
PinDrvKey3=TRUE;
if(PinScanKey1==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey1==TRUE)
	return 3;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey2==TRUE)
	return 7;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey3==TRUE)
	return 10;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey4==TRUE)
	return 14;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
PinDrvKey3=FALSE;				//�������ɨ�������
PinDrvKey4=TRUE;
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey1==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey1==TRUE)
	return 4;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey2==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey2==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey2==TRUE)
	return 8;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey3==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey3==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey3==TRUE)
	return 11;}					//ȷ��ȷʵ�а����źţ����ؼ�ֵ
if(PinScanKey4==TRUE)
	PinLamp=FALSE;
	PinSpeaker=FALSE;
	delay(DELAY_VALUE);		//����ж����źţ�����ʱһ��
	PinLamp=TRUE;
	PinSpeaker=TRUE; 
if(PinScanKey4==TRUE)
{ delay(DELAY_VALUE);			//�����жϻ����źţ�������ʱ
if(PinScanKey4==TRUE)
	return 15;}					//�����жϻ����źţ�������ʱ
PinDrvKey3=FALSE;				//�������ɨ�������
return 16;
}


void delay(short i)
{
	int j=0;
	int k=0;
	k=i*DELAY_VALUE;
	while (j<k)j++;
}
