#include<REG66x.H>
/* ������������ */


/*******************************************
            ��������
����:����I2C���ߵĳ�ʼ��--����ʱ������,I2Cʹ��,������ʼ�ź�
********************************************/
void getbus()
{
	S1CON = 0x43;    // ����ʱ��Ϊ75k(12M),ENS1��λ
	STA = 1;    // ������������,��������
	while(SI == 0);    // �ȴ���ʼλ�ķ���
}


/*******************************************
            �������ݺ���
����:���������߷�������
********************************************/
void sendbyte(unsigned char c)
{
	S1DAT = c;    // ��Ҫ���͵�����װ��S1DAT
	S1CON = 0x43;    // ���SIλ
	while(SI == 0);    // �ȴ����ݷ���
}

/*******************************************
            �����ӵ�ַ���������ֽ����ݺ���
����:���������ߵ����͵�ַ,����,�������ߵ�ȫ����,��������ַsla,
�����͵�����c,���������1��ʾ�����ɹ�,�����ʾ��������.
********************************************/
bit isendbyte(unsigned char sla, unsigned char c)
{
	getbus();    // ��������
	sendbyte(sla);    // ���ʹ�������ַ,����Ӧ���򷵻�
	if(S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(c);    // ��������
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x43;    // ��������
	return 1;
}

/*********************************************************
            �����ӵ�ַ�������Ͷ��ֽ����ݺ���
����:���������ߵ����͵�ַ,�ӵ�ַ,����,�������ߵ�ȫ����,��������ַsla
�ӵ�ַsuba,����������sָ�������,����no���ֽ�.�������1��ʾ�����ɹ�,
�����������.
**********************************************************/
bit isendstr(unsigned char sla, unsigned char suba,
			 unsigned char *s, unsigned char no)
{
	unsigned char i;

	getbus();    // ��������
	sendbyte(sla);    // ���ʹ�������ַ
	if (S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(suba);    // ���������ӵ�ַ
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	for(i=0; i<no; i++)
	{
		sendbyte(*s);    // ��������
		if(S1STA != 0x28)
		{
			S1CON = 0x53;
			return 0;
		}
		s++;
	}
	S1CON = 0x53;
	return 1;
}

/*********************************************************
         �����ӵ�ַ�������ֽ����ݺ���
����: ���������ߵ����͵�ַ,������,�������ߵ�ȫ����,������
��ַsla,����ֵc,�������1��ʾ�����ɹ�,�����������.
***********************************************************/
bit irecvbyte(unsigned char sla, unsigned char *c)
{
	gerbus();    // ��������
	sendbyte(sla+1);    // ����������ַ
	if(S1STA != 0x40)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x43;    // ����һ�ֽ����ݼ����ͷ�Ӧ��λ
	while(SI == 0);    // �ȴ���������
	if(S1STA != 0x58)
	{
		S1CON=0x53;
		return 0;
	}
	*c = S1DAT;
	S1CON = 0x53;
	return 1;
}

/*********************************************************
        �����ӵ�ַ������ȡ���ֽ����ݺ���
����:���������ߵ����͵�ַ,�ӵ�ַ,������,�������ߵ�ȫ����,��
������ַsla,�ӵ�ַsuba,�������ݷ���sָ��Ĵ洢��,��no���ֽ�.
�������1��ʾ�����ɹ�,�����������.
**********************************************************/
bit irecvstr(unsigned char sla, unsigned char suba,
			unsigned char *s, unsigned char no)
{
	unsigned char i;
	getbus();    // ��������
	sendbyte(sla);    // ���ʹ�������ַ
	if(S1STA != 0x18)
	{
		S1CON = 0x53;
		return 0;
	}
	sendbyte(suba);    // ���������ӵ�ַ
	if(S1STA != 0x28)
	{
		S1CON = 0x53;
		return 0;
	}
	S1CON = 0x63;    // ������������
	while(SI == 0);
	sendbyte(sla+1);
	if(S1STA != 0x40)
	{
		S1CON = 0x53;
		return 0;
	}
	for(i=0; i<no; i++)
	{
		S1CON = 0x47;    // ����һ���ֽ����ݲ�����Ӧ��λ
		while(SI == 0);    // �ȴ���������
		if(S1STA != 0x50)
		{
			S1CON = 0x53;
			return 0;
		}
		*s=S1DAT;    // ��ȡ����
		s++;
	}
	S1CON = 0x43;    // �������һ�ֽ����ݲ����ͷ�Ӧ��λ
	while(SI == 0);
	*s = S1DAT;
	S1CON = 0x53;    // ��������
	return 1;
}

/***********************************************************************
                ����ʱ��оƬpcf8573
 * �ӵ�ַ��ʽ:        msb    1  1  0  1  0  A1  A0  R/~W
               A1 A0ΪӲ��ʵ����ӵ�ַ
 * ������֡��ʽ:      SLA+R/~W  A  MODE POINTER  A    DATA    A       P
                                                     nλ����     ||{�Զ�b1,b0����} 
    MODE POINTER:    0  c2  c1  c0  0 b2  b1  b0
    �京��:     0  c2  c1  c0  
                0   0   0   0     execute address
                0   0   0   1     read control/status flags
                0   0   1   0     reset prescaler, including seconds counter; without carry for minute counter
                0   0   1   1     time adjust, with carry for minute counter (note 1)
                0   1   0   0     reset NODA flag
                0   1   0   1     set NODA flag
                0   1   1   0     reset COMP flag
                
                
                0  b2  b1  b0
                0   0   0   0     time counter hour
                0   0   0   1     time counter minutes
                0   0   1   0     time counter days
                0   0   1   1     time counter months
                0   1   0   0     alarm register hours
                0   1   0   1     alarm register minutes
                0   1   1   0     alarm register days
                0   1   1   1     alarm register months
    ֻ�е�c2,c1,c0Ϊ0,0,0ʱ,���ݲź�ִ�е�ַ��ƥ��.
                MSB DATA LSB
        ADDRESSED TO: UPPER DIGIT LOWER DIGIT
             UD UC UB UA LD LC LB LA
                0 0 D D D D D D hours
                0 D D D D D D D minutes
                0 0 D D D D D D days
                0 0 0 D D D D D months
                0 0 0 m s NODA COMP POWF control/status flags
*********************************************************************************/


     
    
               
                
                
                
                
                
                
                
