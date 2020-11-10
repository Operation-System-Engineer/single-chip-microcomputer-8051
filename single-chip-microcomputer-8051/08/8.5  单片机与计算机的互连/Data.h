#include "port.h"

class CData
{
public:
        CPort m_port;
	    BOOL senddata(char *datastr);
		//送数据
		BOOL recvdata();
		//接收数据，0x11为正确，0x88为错误
		BOOL InitPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits);
		BOOL outtemperature(char *datastr);
        CData();
		virtual ~CData();
		
};
