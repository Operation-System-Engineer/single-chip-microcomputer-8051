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
	//返回端口状态,是否已经初始化
	BOOL Inited();
	
	//清缓存
	void ClearBuffer();
	
	//从端口读一个字符,如果没有则等到有为止
	BOOL WaitChar(unsigned char * pCharData);
	
	//从端口读nNumToRead个字符,存在以pDataAddr为首地址的缓存中
	//m_nWait表示输入缓存中如果没有足够的数据,等待时间的长度
	BOOL ReadData(LPVOID pDataAddr,DWORD nNumToRead,UINT m_nWait=500);
	
	//从端口读一个字符,cReadData接收字符,cParity接收第九位,nWait-如果缓冲中没有字符,等待时间的长度
	BOOL ReadChar(unsigned char *cReadData,UINT nWait=100);
	
	//往端口写字符,cWriteData为要写字符的指针
	BOOL WriteChar(unsigned char cWriteData);
	
	//往端口写字符串，nNumToWrite为要写字符串的长度，pDataAddr为字符串首地址
	BOOL WriteData(LPVOID pDataAddr,DWORD nNumToWrite);
	
	//初始化端口
	BOOL Init1(UINT portnr,DWORD dwCommEvents,UINT writebuffersize);
	BOOL InitPort(UINT portnr,DCB &dcb,DWORD dwCommEvents,UINT writterbuffersize);
	BOOL InitPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits,DWORD dwCommEvents,UINT writebuffersize);
	
	CPort();
	virtual ~CPort();

protected:
	//端口是否已经初始化了
	BOOL m_bInited;
	
	//同步变量
	OVERLAPPED          m_ov;
	
//	DCB					m_dcb;
	
	//端口句柄
	HANDLE              m_hComm;
	
	COMMTIMEOUTS		m_CommTimeouts;
	
	//端口号
	UINT				m_nPortNr;
	
	//写缓存
	char*				m_szWriteBuffer;
	
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;
};

#endif // !defined(AFX_PORT_H__EA70F052_D6AE_11D3_841C_0000B4B5BC6E__INCLUDED_)
