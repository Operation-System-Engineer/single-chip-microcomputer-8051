/* p2p_s.c */
/* ˫����Ե�ͨ�ŵĴӻ����� */
#ifndef __P2P_S_C__
#define __P2P_S_C__

#include <AT89X51.H>
#include <STRING.H>

#define __MAX_LEN_ 64    // ������󳤶�
#define _MHZ_ 11    // ���õ�Ƭ��ʹ�õľ���Ƶ��(11.0592MHz)

/* ����Ϊ����Э����ʹ�õ������ź� */
#define __RDY_ 0x06    // ������ʼͨ��ʱ���͵ĺ����ź�
#define __BUSY_ 0x15    // �ӻ�æӦ��
#define __OK_ 0x00    // �ӻ�׼����
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

	/* ���ڳ�ʼ�� */
	init_serial();    // ��ʼ������
	EA = 0;    // �ر������ж�

	/* �����豸Ӧ��׶� */
	while(1)
	{
		/* ������յ������ݲ��������ź�__RDY_��������ȴ� */
		while(tmp != __RDY_)
		{
			RI = 0;
			while(!RI);
			tmp = SBUF;
			RI = 0;
		}
		/* ����ͨ�����P0�������жϵ�ǰ�豸״̬����P0=0xab����ʾ��ǰ�豸æ */
		P0 = 0xff;
		tmp = P0;
		if(tmp == 0xab)    // ���P0������Ϊ0x,��ǰ�豸æ������__BUSY_�ź�
		{
			TI = 0;
			SBUF = __BUSY_;
			while(!TI);
			TI = 0;
			continue;
		}
		TI = 0;    // ������__OK_�źű�ʾ���Խ�������
		SBUF = __OK_;
		while(!TI);
		TI = 0;
		/* ���ݽ��� */
		tmp = 0xff;
		while(tmp == 0xff)
		{
			tmp = recv_data(buf);    // У��ʧ�ܷ���0xff�����ճɹ��򷵻�0
		}
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
	SCON = 0x50;	//������ʽ1��������9600bps���������
}

/* �������ݣ�ע��ú���ʹ��bufָ��Ļ������������ݣ�������ĩβʹ��'\0'��ʾ���ݽ���
 * ����ֵΪ0������У��ɹ�������ֵΪ0xff������У��ʧ��  
 */
unsigned char recv_data(unsigned char *buf)
{
	unsigned char len;    // ���ֽ����ڱ������ݳ���
	unsigned char ecc;    // ���ֽ����ڱ���У���ֽ�
	unsigned char i,tmp;
	/* �������ݳ��� */
	RI = 0;
	while(!RI);
	len = SBUF;
	RI = 0;
	/* ʹ��len��ֵΪУ���ֽ�ecc����ֵ */
	ecc = len;
	/* �������� */
	for(i=0; i<len; i++)
	{
		while(!RI);
		*buf = SBUF;    // ��������
		ecc = ecc^(*buf);    // �����ֽ�У��
		RI = 0;
		buf++;
	}
	*buf = 0;    // ��ʾ���ݽ���
	/* ����У���ֽ� */
	while(!RI);
	tmp = SBUF;
	RI = 0;
	/* ��������У�� */
	ecc = tmp^ecc;
	if(ecc != 0)    // У��ʧ��
	{
		*(buf-len) = 0;    // ������ݻ�����
		TI = 0;    // ����У��ʧ���ź�
		SBUF = __ERR_;
		while(!TI);
		TI = 0;
		return 0xff;    // ����0xff��ʾУ�����
	}
	TI = 0;    // У��ɹ�
	SBUF = __SUCC_;
	while(!TI);
	TI = 0;
	return 0;    // У��ɹ�������0
}


#endif