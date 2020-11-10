// Port.cpp: implementation of the CPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Port.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef _DEBUG
#define DebugMsgStr(a) AfxMessageBox(a)
#else
#define DebugMsgStr(a) 0
#endif 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPort::CPort()
{
	//��ʼ������
	m_hComm = NULL;
	m_szWriteBuffer = NULL;
	m_bInited=FALSE;

	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;
	m_ov.hEvent = NULL;
}

CPort::~CPort()
{
	CloseHandle(m_hComm);
}

//��ʼ���˿�
BOOL CPort::Init1(UINT portnr,DWORD dwCommEvents,UINT writebuffersize)
{
	//�˿ں�ֻ����1��4
	if (portnr<1||portnr>4)
	{
		DebugMsgStr("�˿ںų�����Χ!");
		return FALSE;
	}

	//����m_ov���¼�
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//BOOL bResult;//�õ��������ý��
	char *szPort = new char[5];//�˿ںŶ�Ӧ���ַ���

	//����Ҫ����ַ���
	sprintf(szPort, "COM%d", portnr);

	//����������и�ֵ
	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];
	m_nPortNr = portnr;
	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;
		
	//���ó�ʱ���
	m_CommTimeouts.ReadIntervalTimeout = 50;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 50;
	m_CommTimeouts.ReadTotalTimeoutConstant = 50;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 50;
	m_CommTimeouts.WriteTotalTimeoutConstant = 50;

	    //����˿��Ѵ��ˣ��Ͱ����ر�
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//�����˿�
	m_hComm = CreateFile(szPort,						// �˿ں��ַ���
					     GENERIC_READ | GENERIC_WRITE,	// ��д����
					     0,								// ���Է���
					     NULL,							// û�а�ȫ����
					     OPEN_EXISTING,					// �򿪷�ʽ
					     FILE_FLAG_OVERLAPPED,			// �첽
					     0);							// ����Ϊ0

	//�������ʧ���򷵻�
	if (m_hComm == INVALID_HANDLE_VALUE)
	{		
		delete [] szPort;
//		delete [] szBaud;
		return FALSE;
	}

	//���ö˿ڵ���������
	//��ʱ
	if (!SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		DebugMsgStr("���ö˿ڳ�ʱ����");
		return FALSE;
	}
	if (!SetCommMask(m_hComm, dwCommEvents))
	{
		DebugMsgStr("���ö˿ڴ����¼�����");
		return FALSE;
	}	
	return TRUE;
}
BOOL CPort::InitPort(UINT portnr,DCB &dcb,DWORD dwCommEvents,UINT writebuffersize)
{
	if(!Init1(portnr,dwCommEvents,writebuffersize))
		return FALSE;
	if (!SetCommState(m_hComm, &dcb))
	{
		DebugMsgStr("���ö˿�DCB����");
		return FALSE;
	}

	//��ջ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	m_bInited=TRUE;
	return TRUE;
}
BOOL CPort::InitPort(UINT portnr,// �˿ںţ�����1��4
					 UINT baud,// �����ʣ���������9600���ȡ�
					 char parity,// ��żУ��λ���ַ�
					             // N����У��  O����У��  E��żУ��
								 // M-����Ϊ1 S-����Ϊ0 ��������һ�����ڶ��ͨ��ģʽ
					 UINT databits,// ����λ���ȣ�����
					 UINT stopbits,// ֹͣλ���ȣ�������1��2
					 DWORD dwCommEvents,// �����¼�
					 UINT writebuffersize)//д����ĳ���
{
	if(!Init1(portnr,dwCommEvents,writebuffersize))
		return FALSE;

	char *szBaud = new char[50];//�����ʶ�Ӧ���ַ���
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	DCB m_dcb;
	if (!GetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("��ͼ�õ�DCB����");
		return FALSE;
	}

	m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!

	if (!BuildCommDCB(szBaud, &m_dcb))
	{
		DebugMsgStr("����DCB����");
		return FALSE;
	}
	if (!SetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("���ö˿�DCB����");
		return FALSE;
	}

	//��ջ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	m_bInited=TRUE;
	return TRUE;
}

//���˿�д�ַ�,cWriteDataΪҪд�ַ���ָ��
BOOL CPort::WriteChar(unsigned char cWriteData)
{
	if (!m_bInited)
	{
		DebugMsgStr("�˿���δ��ʼ��!");
		return FALSE;
	}

	//��ʼ������
	DWORD BytesSent = 0;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;	

	BOOL bResult;
	//д�ַ�
	bResult=WriteFile(m_hComm,&cWriteData,1,&BytesSent,&m_ov);

	if (bResult)
	{
		//�ɹ��򷵻�
		return TRUE;
	}

	//�õ��������
	DWORD dwError = GetLastError();

	//���������δ����򷵻ش����ź�
	if (dwError!=ERROR_IO_PENDING)
	{
		DebugMsgStr("д�ַ�����");
		return FALSE;
	}
	
	//��δ�����ȴ����
	if (!GetOverlappedResult(m_hComm,&m_ov,&BytesSent,TRUE))
	{
		//ʧ�ܷ��ش����ź�
		DebugMsgStr("д�ַ�����");
		return FALSE;
	}

	//�ɹ�
	return TRUE;
}

//���˿�д�ַ�����nNumToWriteΪҪд�ַ����ĳ��ȣ�pDataAddrΪ�ַ����׵�ַ
BOOL CPort::WriteData(LPVOID pDataAddr,DWORD nNumToWrite)
{
	if (!m_bInited)
	{
		DebugMsgStr("�˿���δ��ʼ��!");
		return FALSE;
	}

	//��ʼ������
	DWORD BytesSent = 0;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;	

	BOOL bResult;
	//д�ַ�
	bResult=WriteFile(m_hComm,pDataAddr,nNumToWrite,&BytesSent,&m_ov);

	if (bResult)
	{
		//�ɹ��򷵻�
		return TRUE;
	}

	//�õ��������
	DWORD dwError = GetLastError();

	//���������δ����򷵻ش����ź�
	if (dwError!=ERROR_IO_PENDING)
	{
		DebugMsgStr("д�ַ�������");
		return FALSE;
	}
	
	//��δ�����ȴ����
	GetOverlappedResult(m_hComm,&m_ov,&BytesSent,TRUE);
	if (BytesSent!=nNumToWrite)
	{
		//ʧ�ܷ��ش����ź�
		DebugMsgStr("û���㹻���ַ���!");
		return FALSE;
	}
	
	//�ɹ�
	return TRUE;
}

//�Ӷ˿ڶ�һ���ַ�,cReadData�����ַ�,nWait-���������û���ַ�,�ȴ�ʱ��ĳ���
BOOL CPort::ReadChar(unsigned char *cReadData,UINT nWait)
{
	BOOL bResult;
	DWORD dwError,dwBytesRead;
	COMSTAT comstat;

	if (!m_bInited)
	{
		DebugMsgStr("�˿���δ��ʼ��!");
		return FALSE;
	}

	//�õ���ǰ���뻺��״̬
	bResult = ClearCommError(m_hComm, &dwError, &comstat);

	//���û�������Ҳ��ȴ��򷵻ش���
	if (comstat.cbInQue==0&&nWait==0)
	{
		DebugMsgStr("CPort::ReadChar-û����,������!");
		return FALSE;
	}

	//���ַ�
	bResult = ReadFile(m_hComm,cReadData,1,&dwBytesRead,&m_ov);

	if (!bResult)
	{
		//���������δ����򷵻ش���
		if (dwError=GetLastError()!=ERROR_IO_PENDING)
		{
			DebugMsgStr("CPort::ReadChar-������!");
			return FALSE;
		}

		if (WaitForSingleObject(m_ov.hEvent,nWait)!=WAIT_OBJECT_0)
		{
			DebugMsgStr("CPort::ReadChar-û�ȵ�(�㹻)����!");
			return FALSE;
		}

	}

	//������ȷ
	return TRUE;	
}

//�Ӷ˿ڶ�nNumToRead���ַ�,������pDataAddrΪ�׵�ַ�Ļ�����
//m_nWait��ʾ���뻺�������û���㹻������,�ȴ�ʱ��ĳ���
BOOL CPort::ReadData(LPVOID pDataAddr,DWORD nNumToRead,UINT m_nWait)
{
	BOOL bResult;
	DWORD dwError,dwBytesRead;
	COMSTAT comstat;

	if (!m_bInited)
	{
		DebugMsgStr("�˿���δ��ʼ��!");
		return FALSE;
	}

	//�õ���ǰ���뻺��״̬
	bResult = ClearCommError(m_hComm, &dwError, &comstat);

	//���û�������Ҳ��ȴ��򷵻ش���
	if (comstat.cbInQue==0&&m_nWait==0)
	{
		DebugMsgStr("CPort::ReadString-û����,������!");
		return FALSE;
	}

	//���ַ�
	bResult = ReadFile(m_hComm,pDataAddr,nNumToRead,&dwBytesRead,&m_ov);

	if (!bResult)
	{
		//���������δ����򷵻ش���
		if (dwError=GetLastError()!=ERROR_IO_PENDING)
		{
			DebugMsgStr("CPort::ReadString-������!");
			return FALSE;
		}
	
		if (WaitForSingleObject(m_ov.hEvent,m_nWait)!=WAIT_OBJECT_0)
		{
			DebugMsgStr("CPort::ReadString-û�ȵ�(�㹻)����!");
			return FALSE;
		}
			
	}

	//������ȷ
	return TRUE;	

}

//�Ӷ˿ڶ�һ���ַ�,���û����ȵ���Ϊֹ
BOOL CPort::WaitChar(unsigned char *pCharData)
{
	DWORD pEvtMask,dwError;
	COMSTAT comstat;
	BOOL bResult;
	unsigned char cGetData;

	//�˿�δ��ʼ������
	if (!m_bInited)
	{
		DebugMsgStr("�˿���δ��ʼ��!");
		return FALSE;
	}

	//�ж����뻺�����Ƿ��Ѿ������ַ�
	bResult=ClearCommError(m_hComm, &dwError, &comstat);
	//���������������ȷ
	if (comstat.cbInQue!=0)
	{
		bResult=ReadChar(&cGetData);

		if (bResult)
		{
			*pCharData=cGetData;
			return TRUE;
		}
	}

	UINT nNum=0;
	//�ȵ�����һ���ַ�Ϊֹ
	while (nNum==0)
	{
		bResult=WaitCommEvent(m_hComm,&pEvtMask,&m_ov);

		if (bResult)
		{
			//���ж��Ƿ��ǳ�����ʱ�������Ĵ���
			bResult = ClearCommError(m_hComm, &dwError, &comstat);
			//�Ǵ������������ѭ��,���µȴ�
			if (comstat.cbInQue==0)
				continue;
		}
		else
			dwError = GetLastError();

		//�ȴ��ַ�����
		WaitForSingleObject(m_ov.hEvent,INFINITE);

		//���ַ�
		bResult=ReadChar(&cGetData);

		if (bResult)
		{
			//�������򷵻���ȷ���
			*pCharData=cGetData;
			nNum++;
		}
		else
		{
			//���˴���,������һ�εȴ�
			DebugMsgStr("WaitChar:read error!");
		}
	}
	return TRUE;
}

//�建��
void CPort::ClearBuffer()
{
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);		
}

//���ض˿�״̬
BOOL CPort::Inited()
{
	return m_bInited;
}

BOOL CPort::SetMark(BOOL bMark)
{
	DCB m_dcb;
	if (!GetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("��ͼ�õ�DCB����");
		return FALSE;
	}	

	if (bMark)
		m_dcb.Parity = MARKPARITY;
	else
		m_dcb.Parity = SPACEPARITY;
	
	if (!SetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("���ö˿�DCB����");
		return FALSE;
	}
	return TRUE;
}