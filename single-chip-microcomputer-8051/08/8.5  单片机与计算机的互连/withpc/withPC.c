#pragma SRC
#include <stdio.h>
#include <reg51.h>
#define		TRUE			1
#define		FALSE			0

#define		LampComm	P0_0
//������䶨����������ͨѶ�����ź�
#define		SelectComm1	P0_1
#define		SelectComm2	P0_2
#define		SelectComm3	P0_3
#define		SelectComm4	P0_4
#define		SelectComm5	P0_5
#define		SelectComm6	P0_6
//������䶨������ͨѶ�Ĵӻ���
#define 	MACHINECOMM_OK 	 0x11  
//������䶨������ͨѶ���������ź�
#define 	MACHINECOMM_RET 0x88    
//������䶨��ӻ��ظ�ͨѶ�����ź�
#define MACHINECOMM_DATAHEAD 	0x5a 
//������䶨�����ݰ���ͷ
#define MACHINECOMM_DATAEND 	0xa5 
//������䶨�����ݰ���β

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


bit EvRcv=FALSE;			//�Ƿ��յ��������ݵı�־λ

unsigned char rcvdata=0;		//�Ӵ��ڴ洢���յı���
void send(char *temp,int j);		//�����ӳ���
void initial(void);     		//��ʼ���ӳ���
void delay(short i);			//��ʱ�ӳ���

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
	SelectComm6=1;//ѡ����PC������ͨ�š�
	while(1)
	{
		if(EvRcv)//������յ������˷��͹����������ź�
		{
			if(rcvdata==MACHINECOMM_OK)
			{   LampComm=TRUE;
			delay(DELAY_VALUE);
			LampComm=FALSE;
			}	//ָʾ����˸		
			EvRcv=FALSE;
			break;//�˳��ȴ�
		}
		send(&datahand,1);//����һ�������ź�
		break;
	}
	for(;;){while(1)
	{if(EvRcv){
		datarecv[count]=rcvdata;
		count++;
		EvRcv=FALSE;
	}
	if(count==5)break;//����Ѿ��յ��������ݣ�����Ϊ���յ�һ�����ݰ�
	}//����һ�����ݰ�
	if(datarecv[0]==MACHINECOMM_DATAHEAD&&datarecv[5]==MACHINECOMM_DATAEND)
		send(datacomm,6);//������ݰ���ȷ���򷵻�һ�����ݰ���
	}
}

void initial()		//��ʼ���ӳ���
{
	IP=0x10;              //���崮��Ϊ�����ȼ��ж�
	IE=0x97;              //�����ڡ��ж�0��1����ʱ��0
	TCON=0x05;
	
	TMOD=0x21;            //��ʱ��1Ϊ�Զ�װ��(auto-load)��ʽ
	PCON=0;               //SMOD(PCON.7)=1ʱ�������ʷ���.//smod=1 (����Ϊ11.0592MʱΪ0)
	
	SCON=0xD0;            //���пڹ�����ʽ : 9λUART, �����ʿɱ�
	TH1=0xf3;
	TL1=0xf3;             //
	PCON=0x80|PCON;	//SMOD=1;		//���������� : 9600 baud(E8--24MHZ)
	//SMOD=0=4.8kbps
	//th1=tl1=fd,smod=0 ,=19.2kbps
	TR1=1;                //������ʱ��1
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
	for(i=0;i<=j-1;i++)	//���������ݵĳ�������������
	{
		ACC=*(temp+i);
		TB8=P;
		SBUF=ACC;	//��������
		while(TI==0);	//�����������ʱ��TI���Զ��ø�
		TI=0;		//����������ϣ���TI���㣬׼����һ�η���
	}
	EA=1;
	LampComm=FALSE;
}

void Rcv_INT(void) interrupt 1
//�����ж����ȼ�Ϊ1
{
	LampComm=TRUE;
	if(RI)		//����յ��������������Ĳ���
	{
		ACC=SBUF;		//������ͨ�ŵĻ����е����ݴ���Ĵ���A
		if(P==RB8)
		{
			rcvdata=ACC;	//���Ĵ���A�е����ݴ������rcvdata
			EvRcv=TRUE;	//���Ѿ����յ����ݵı�־λ�ø�
		}
		RI=0;		//RI����
	}
	LampComm=FALSE;
}

//��ʱ�ӳ���
void delay(int i)
{
	int j=0;
	i=i*DELAY_VALUE;
	while (j<i)j++;
}