#include "port.h"

class CData
{
public:
        CPort m_port;
	    BOOL senddata(char *datastr);
		//������
		BOOL recvdata();
		//�������ݣ�0x11Ϊ��ȷ��0x88Ϊ����
		BOOL InitPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits);
		BOOL outtemperature(char *datastr);
        CData();
		virtual ~CData();
		
};
