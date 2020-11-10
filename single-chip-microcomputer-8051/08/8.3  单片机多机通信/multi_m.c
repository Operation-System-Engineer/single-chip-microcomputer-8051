/* multi_m.c */
/* ���ͨ�ŵ��������� */
#ifndef __MULTI_M_C__
#define __MULTI_M_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // ������󳤶�
#define _MHZ_ 11    // ���õ�Ƭ��ʹ�õľ���Ƶ��(11.0592MHz)

/* ����Ϊ����Э����ʹ�õ������ź� */
#define __SUCC_ 0x0f    // ���ݴ��ͳɹ�
#define __ERR_ 0xf0    // ���ݴ��ʹ���

void init_serial();    // ���ڳ�ʼ��
void send_data(unsigned char *buf);    // ��������
void delay10ms(unsigned int count);    // ��ʱ�ӳ���(10ms)

void main()
{
	char buf[__MAX_LEN_];
	unsigned char i = 0;
	unsigned char tmp;
	unsigned char addr;    // ���ֽ����ڱ���Ҫͨ�ŵĴӻ���ַ

	/* Ϊ����������ֵ */
	P0 = 0xff;
	while(P1 != 0)    // ÿ��100ms��P0�ڶ�ȡ������ȡ��0��������ݲɼ�����
	{
		*(buf+i) = P0;
		delay10ms(10);    // ��ʱ100ms
		P0 = 0xff;
		i++;
	}
	*(buf+i) = 0;    // ���������һ���ֽ�Ϊ0��ʾ���ݽ���
	/* ��Ҫ���ʵķֻ���ַ */
	P0 = 0xff;
	addr = P0;
	/* ���ڳ�ʼ�� */
	init_serial();    // ��ʼ������
	EA = 0;    // �ر������ж�

	/* ���͵�ַ֡������Ӧ����Ϣ��������յ��ź��뷢�͵ĵ�ַ��Ϣ��ͬ�������·��͵�ַ֡ */
	tmp = addr-1;
	while(tmp != addr)
	{
		/* ���ʹӻ���ַ */
		TI = 0;
		TB8 = 1;    // ���͵�ַ֡
		SBUF = addr;
		while(!TI);
		TI = 0;
		/* ���մӻ�Ӧ�� */
		RI = 0;
		while(!RI);
		tmp = SBUF;
		RI = 0;
	}
	/* �������ݲ�����У����Ϣ��������յ��ź�Ϊ0FH����ʾ�ӻ����ճɹ����������·��͸������� */
	tmp = __ERR_;
	while(tmp != __SUCC_)
	{
		send_data(buf);    // ��������
		RI = 0;
		while(!RI);
		tmp = SBUF;
		RI = 0;
	}
	while(1);    // ���������������ѭ��
}

/* ��ʼ������ */
void init_serial()
{
	TMOD = 0x20;	//��ʱ��T1ʹ�ù�����ʽ2
	TH1 = 250;    // ���ó�ֵ
	TL1 = 250;
	TR1 = 1;    // ��ʼ��ʱ
	PCON = 0x80;    // SMOD = 1
	SCON = 0xd0;	//������ʽ3��9λ����λ��������9600bps���������
}

/* �������� */
void send_data(unsigned char *buf)
{
	unsigned char len;    // �������ݳ���
	unsigned char ecc;    // ����У���ֽ�
	
	len = strlen(buf);    // ����Ҫ�������ݵĳ���
	ecc = len;    // ��ʼ����У���ֽڼ���
	/* �������ݳ��� */
	TI = 0;    
	TB8 = 0;    // ��������֡
	SBUF = len;    // ���ͳ���
	while(!TI);
	TI = 0;
	/* �������� */
	for(i=0; i<len; i++)
	{
		ecc = ecc^(*buf);    // ����У���ֽ�
		TB8 = 0;    // ��������֡
		SBUF = *buf;    // ��������
		buf++;
		while(!TI);
		TI = 0;
	}
	/* ����У���ֽ� */
	TB8 = 0;    // ��������֡
	SBUF = ecc;    // ����У���ֽ�
	while(!TI);
	TI = 0;
}

/* ��ʱ10ms�����Ƚϵͣ�����countΪ��ʱʱ�� */
void delay10ms(unsigned int count)
{
	unsigned int i, k;
	unsigned char j;
	unsigned int tmp;
	tmp = (int)((100*_MHZ_)/12);

	for(i=0; i<count; i++)
		for(j=0; j<100; j++)
			for(k=0; k<tmp; k++);
}

#endif