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
unsigned char keyscan(void);
//����ɨ���ӳ���
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
unsigned char sets0=0;  //�趨���ӵ��ֶ�ʱ��,�趨Ϊ����ʱ�䣬����ʱ�䶨Ϊ1����

void main(void)
{
	unsigned char status;
	unsigned char keyword;
	initial();
	//���������źţ���������ܣ��Լ���������������Ч״̬
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
					PinSpeaker=TRUE;     //�������Ӷ�ʱʱ�䣬��������
				}
				else
					delay(DELAY_VALUE_EXTRA);
				updatee();
			}
			else
				if(keyword==11)status=1; //0״̬Ϊ��ʱ״̬��1״̬Ϊֹͣ״̬
				else 
				{
					PinSpeaker=FALSE;
					delay(DELAY_VALUE);
					PinSpeaker=TRUE;     //�����д���������
				}
				break;
		case 1:
			keyword=keyscan();
			if(keyword==10)status=0;
			else if(keyword==12)status=2;//2״̬Ϊ���ó�ֵ״̬��
			else if(keyword==13){h0=0;
			h1=0;
			s0=0;
			s1=0;
			m0=0;
			m1=0;}//����
			else if(keyword==14)status=3;//3״̬Ϊ��������ʱ��״̬��
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //�����д���������   			
			}
			break;
		case 2:
			keyword=keyscan();
			if(keyword==10&&s1<=5&&m1<=5&&(h1*10+h0)<24)
				status=0;
			else if(keyword==11&&s1<=5&&m1<=5&&(h1*10+h0)<24)
				status=1;
			else if(keyword<10&&keyword>=0) //����������ּ�
			{
				h1=h0;
				h0=m1;
				m1=m0;
				m0=s1;
				s1=s0;
				s0=keyword;
				updatee();		   //��������ּ��ӵ�λ��������
			}
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //�����д���������   			
			}
		case 3:
			keyword=keyscan();
			if(keyword==10&&sets1<=5&&setm1<=5&&(seth1*10+seth0)<24)
				status=0;
			else if(keyword==11&&sets1<=5&&setm1<=5&&(seth1*10+seth0)<24)
				status=1;
			else if(keyword<10&&keyword>=0) //����������ּ�
			{
				seth1=seth0;
				seth0=setm1;
				setm1=setm0;
				setm0=sets1;
				sets1=sets0;
				sets0=keyword;
				updatee();		   //��������ּ��ӵ�λ��������
			}
			else
			{
				PinSpeaker=FALSE;
				delay(DELAY_VALUE);
				PinSpeaker=TRUE;     //�����д���������   			
			}
			
		}	
	}
	
}

void initial(void)
{
	P1_0=FALSE;
	P1_1=FALSE;
	P1_2=FALSE;
	P1_3=FALSE;			//���������źŶ���Ϊ��,������š�  
	P0_0=TRUE;			//�����������Ϊ����
	P0_1=TRUE;			//����������
}

void delay(short i)	//�ӳ��ӳ���Ϊ��ȷ��ʱ����Ҫ�û������������R6R7�պô����i��ֵ
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
	PinDrvKey1=FALSE;	//ɨ���һ��
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
	PinDrvKey2=FALSE;	//ɨ��ڶ���
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
	PinDrvKey3=FALSE;	//ɨ�������
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
	PinDrvKey3=FALSE;	//ɨ�������
	return 16;
}

void updatee()
{
	if(s0==9)	//���10msλ��Ҫ��λ����������
	{
		s0=0;
		if(s1==5)	//���100msλ��Ҫ��λ����������
		{
			s1=0;
			if(m0==9)	//���1secλ��Ҫ��λ����������
			{
				m0=0;
				if(m1==5)	//���10secλ��Ҫ��λ����������
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
	delay(DELAY_VALUE); //��������������������ʾ��������
	
}