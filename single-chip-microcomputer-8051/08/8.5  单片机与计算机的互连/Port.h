// Port.h: interface for the CPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORT_H__EA70F052_D6AE_11D3_841C_0000B4B5BC6E__INCLUDED_)
#define AFX_PORT_H__EA70F052_D6AE_11D3_841C_0000B4B5BC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPort  
{
public:
	BOOL SetMark(BOOL bMark=TRUE);
	//���ض˿�״̬,�Ƿ��Ѿ���ʼ��
	BOOL Inited();
	
	//�建��
	void ClearBuffer();
	
	//�Ӷ˿ڶ�һ���ַ�,���û����ȵ���Ϊֹ
	BOOL WaitChar(unsigned char * pCharData);
	
	//�Ӷ˿ڶ�nNumToRead���ַ�,������pDataAddrΪ�׵�ַ�Ļ�����
	//m_nWait��ʾ���뻺�������û���㹻������,�ȴ�ʱ��ĳ���
	BOOL ReadData(LPVOID pDataAddr,DWORD nNumToRead,UINT m_nWait=500);
	
	//�Ӷ˿ڶ�һ���ַ�,cReadData�����ַ�,cParity���յھ�λ,nWait-���������û���ַ�,�ȴ�ʱ��ĳ���
	BOOL ReadChar(unsigned char *cReadData,UINT nWait=100);
	
	//���˿�д�ַ�,cWriteDataΪҪд�ַ���ָ��
	BOOL WriteChar(unsigned char cWriteData);
	
	//���˿�д�ַ�����nNumToWriteΪҪд�ַ����ĳ��ȣ�pDataAddrΪ�ַ����׵�ַ
	BOOL WriteData(LPVOID pDataAddr,DWORD nNumToWrite);
	
	//��ʼ���˿�
	BOOL Init1(UINT portnr,DWORD dwCommEvents,UINT writebuffersize);
	BOOL InitPort(UINT portnr,DCB &dcb,DWORD dwCommEvents,UINT writterbuffersize);
	BOOL InitPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits,DWORD dwCommEvents,UINT writebuffersize);
	
	CPort();
	virtual ~CPort();

protected:
	//�˿��Ƿ��Ѿ���ʼ����
	BOOL m_bInited;
	
	//ͬ������
	OVERLAPPED          m_ov;
	
//	DCB					m_dcb;
	
	//�˿ھ��
	HANDLE              m_hComm;
	
	COMMTIMEOUTS		m_CommTimeouts;
	
	//�˿ں�
	UINT				m_nPortNr;
	
	//д����
	char*				m_szWriteBuffer;
	
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;
};

#endif // !defined(AFX_PORT_H__EA70F052_D6AE_11D3_841C_0000B4B5BC6E__INCLUDED_)
