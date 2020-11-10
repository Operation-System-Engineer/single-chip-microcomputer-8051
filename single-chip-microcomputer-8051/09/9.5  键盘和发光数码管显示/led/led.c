#include <stdio.h>
#include <reg51.h>


#define		TRUE			1
#define		FALSE			0

#define		DELAY_VALVE		3




#define    	PinDrvKey1	P1_0
#define		PinDrvKey2	P1_1
#define		PinDrvKey3	P1_2
#define    	PinDrvKey4	P1_3		
//����ɨ���������


#define    	PinScanKey1	P1_4
#define    	PinScanKey2	P1_5
#define    	PinScanKey3	P1_6
#define    	PinScanKey4	P1_7		
//����ɨ����źŻ�ȡ��

#define		PinSegA		P2_0	
#define		PinSegB		P2_1
#define		PinSegC		P2_2
#define		PinSegD		P2_3		
//�߶����������

#define		PinSelectA	P2_4
#define		PinSelectB	P2_5
#define		PinSelectC	P2_6		
//ѡ����������λ

#define		PinSelectCS	P2_4		
//ѡ������Ƭѡλ

#define		LedSelect0	0x80
#define		LedSelect1	0x90
#define		LedSelect2	0xa0
#define		LedSelect3	0xb0
#define		LedSelect4	0xc0
#define		LedSelect5	0xd0

#define		PinLamp		P0_0		
//ָʾ�������ܽ�
#define		PinSpeaker	P0_1		
//�����������ܽ�

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
//��ʼ���ӳ���
void delay(short i);		 
//��ʱ�ӳ���
void display(unsigned char i,unsigned char num);
//��ʾ�ӳ������б���i,num����Ϊ�ڵ�i���������ʾ����num.

void main(void)
{
	unsigned char i;
	initial();			
//���������źţ���������ܣ��Լ���������������Ч״̬
	while(1)
	{
		for(i=0;i<=5;i++) 
		{
			display(i,1);
			delay(DELAY_VALVE);
			display(i,8);
//�����������ʾ��Ϊ8��ֻ������1�ڸ��������֮��ѭ������
		}
		
	}
	
}

void initial(void)
{
	P1_0=FALSE;
	P1_1=FALSE;
	P1_2=FALSE;
	P1_3=FALSE;			
//���������źŶ���Ϊ��,������š�
	P0_0=TRUE;			
//�����������Ϊ����
	P0_1=TRUE;		
//����������
}

void delay(short i)
{
	int j=0;
	int k=0;
	k=i*DELAY_VALVE;
	while (j<k)j++;
}

void display(unsigned char i,unsigned char num)
{
	if(num >= 0 && num <=9)
	{switch(i)
	{ case 0:P2= LedSelect0 | num;  
//LedSelect0�ĸ���λΪѡ���0������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
	break;
	  case 1:P2= LedSelect1 | num;
//LedSelect1�ĸ���λΪѡ���1������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
		  break;
	  case 2:P2= LedSelect2 | num;
//LedSelect2�ĸ���λΪѡ���2������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
		  break;
	  case 3:P2= LedSelect3 | num;
//LedSelect3�ĸ���λΪѡ���3������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
		  break;
	  case 4:P2= LedSelect4 | num;
//LedSelect4�ĸ���λΪѡ���4������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
		  break;
	  case 5:P2= LedSelect5 | num;
//LedSelect5�ĸ���λΪѡ���5������ܵ��߼�������λ��Ϊ�������ʾ�����֡�
		  break;
	  default:break;
	}
	}
}

