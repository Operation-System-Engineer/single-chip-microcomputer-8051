/* multi_s.c */
/* ���ͨ�ŵĴӻ����� */
#ifndef __MULTI_S_C__
#define __MULTI_S_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // ������󳤶�
#define _MHZ_ 11    // ���õ�Ƭ��ʹ�õľ���Ƶ��(11.0592MHz)

/* ����Ϊ����Э����ʹ�õ������ź� */
#define __SUCC_ 0x0f    // ���ݴ��ͳɹ�
#define __ERR_ 0xf0    // ���ݴ��ʹ���

void init_serial();    // ���ڳ�ʼ��
unsigned char recv_data(unsigned char *buf);    // ��������
void Beep_ok();    // ������ʾ���ݽ���ok���ú�������δ����

void main()
{
	char buf[__MAX_LEN_];
	unsigned char i = 0;
	unsigned char tmp = 0xff;
	unsigned char addr;    // ���汾����ַ

	/* ��P1�ڶ�ȡ������ַ */
	P1 = 0xff;
	addr = P1;
	
	/* ���ڳ�ʼ�� */
	init_serial();    // ��ʼ������
	EA = 0;    // �ر������ж�

	/* �����豸Ӧ��׶� */
	while(1)
	{
		SM2 = 1;    // ֻ���յ�ַ֡
		/* ������յ��ĵ�ַ֡���Ǳ�����ַ��������ȴ� */
		tmp = addr-1;
		while(tmp != addr)
		{
			RI = 0;
			while(!RI);
			tmp = SBUF;
			RI = 0;
		}
		/* ����Ӧ���źţ������ý������ݵ�׼�� */
		TI = 0;
		TB8 = 0;    // ����������λ
		SBUF = addr;
		while(!TI);
		TI = 0;
		SM2 = 0;    // �������������Ϣ
		/* ���ݽ��� */
		tmp = 0xff;
		while(tmp == 0xff)    // �������У��ʧ�������½�������
		{
			tmp = recv_data(buf);    // У��ʧ�ܷ���0xff����⵽��ַ֡�򷵻�0xfe�����ճɹ��򷵻�0
		}
		if(tmp == 0xfe)    // �����ݽ��չ����У�������ֵ�ַ֡�������¿�ʼ�������չ���
			continue;
		Beep_ok();    // ������ʾ���ݽ��ճɹ�	
	}
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

/* �������ݣ�ע��ú���ʹ��bufָ��Ļ������������ݣ�������ĩβʹ��'\0'��ʾ���ݽ���
 * ����ֵΪ0������У��ɹ�������ֵΪ0xfe�����ܹ����н��յ���ַ֡������ֵΪ0xff������У��ʧ��  
 */
unsigned char recv_data(unsigned char *buf)
{
	unsigned char len;    // ���ֽ����ڱ������ݳ���
	unsigned char ecc;    // ���ֽ����ڱ���У���ֽ�
	unsigned char i,tmp;
	/* �������ݳ��� */
	RI = 0;
	while(!RI);
	if(RB8 == 1)    // ����ǰ����Ϊ��ַ֡�򷵻�0xfe
		return 0xfe;
	len = SBUF;
	RI = 0;
	/* ʹ��len��ֵΪУ���ֽ�ecc����ֵ */
	ecc = len;
	/* �������� */
	for(i=0; i<len; i++)
	{
		while(!RI);
		if(RB8 == 1)    // ����ǰ����Ϊ��ַ֡�򷵻�0xfe
			return 0xfe;		
		*buf = SBUF;    // ��������
		ecc = ecc^(*buf);    // �����ֽ�У��
		RI = 0;
		buf++;
	}
	*buf = 0;    // ��ʾ���ݽ���
	/* ����У���ֽ� */
	while(!RI);
	if(RB8 == 1)    // ����ǰ����Ϊ��ַ֡�򷵻�0xfe
		return 0xfe;
	tmp = SBUF;
	RI = 0;
	/* ��������У�� */
	ecc = tmp^ecc;
	if(ecc != 0)    // У��ʧ��
	{
		*(buf-len) = 0;    // ������ݻ�����
		TI = 0;    // ����У��ʧ���ź�
		TB8 = 0;
		SBUF = __ERR_;
		while(!TI);
		TI = 0;
		return 0xff;    // ����0xff��ʾУ�����
	}
	TI = 0;    // У��ɹ�
	TB8 = 0;
	SBUF = __SUCC_;
	while(!TI);
	TI = 0;
	return 0;    // У��ɹ�������0
}

#endif