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
	//初始化数据
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

//初始化端口
BOOL CPort::Init1(UINT portnr,DWORD dwCommEvents,UINT writebuffersize)
{
	//端口号只能是1到4
	if (portnr<1||portnr>4)
	{
		DebugMsgStr("端口号超出范围!");
		return FALSE;
	}

	//设置m_ov的事件
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//BOOL bResult;//得到函数调用结果
	char *szPort = new char[5];//端口号对应的字符串

	//生成要求的字符串
	sprintf(szPort, "COM%d", portnr);

	//对类变量进行赋值
	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];
	m_nPortNr = portnr;
	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;
		
	//设置超时间隔
	m_CommTimeouts.ReadIntervalTimeout = 50;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 50;
	m_CommTimeouts.ReadTotalTimeoutConstant = 50;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 50;
	m_CommTimeouts.WriteTotalTimeoutConstant = 50;

	    //如果端口已打开了，就把它关闭
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//创建端口
	m_hComm = CreateFile(szPort,						// 端口号字符串
					     GENERIC_READ | GENERIC_WRITE,	// 读写类型
					     0,								// 绝对访问
					     NULL,							// 没有安全属性
					     OPEN_EXISTING,					// 打开方式
					     FILE_FLAG_OVERLAPPED,			// 异步
					     0);							// 必须为0

	//如果创建失败则返回
	if (m_hComm == INVALID_HANDLE_VALUE)
	{		
		delete [] szPort;
//		delete [] szBaud;
		return FALSE;
	}

	//设置端口的其它属性
	//超时
	if (!SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		DebugMsgStr("设置端口超时错误！");
		return FALSE;
	}
	if (!SetCommMask(m_hComm, dwCommEvents))
	{
		DebugMsgStr("设置端口触发事件错误！");
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
		DebugMsgStr("设置端口DCB错误！");
		return FALSE;
	}

	//清空缓存
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	m_bInited=TRUE;
	return TRUE;
}
BOOL CPort::InitPort(UINT portnr,// 端口号，数字1－4
					 UINT baud,// 波特率，整数，如9600，等。
					 char parity,// 奇偶校验位，字符
					             // N－无校验  O－奇校验  E－偶校验
								 // M-保持为1 S-保持为0 ，这两项一般用于多机通信模式
					 UINT databits,// 数据位长度，整数
					 UINT stopbits,// 停止位长度，整数，1或2
					 DWORD dwCommEvents,// 触发事件
					 UINT writebuffersize)//写缓存的长度
{
	if(!Init1(portnr,dwCommEvents,writebuffersize))
		return FALSE;

	char *szBaud = new char[50];//波特率对应的字符串
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	DCB m_dcb;
	if (!GetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("试图得到DCB错误！");
		return FALSE;
	}

	m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!

	if (!BuildCommDCB(szBaud, &m_dcb))
	{
		DebugMsgStr("生成DCB错误！");
		return FALSE;
	}
	if (!SetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("设置端口DCB错误！");
		return FALSE;
	}

	//清空缓存
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	m_bInited=TRUE;
	return TRUE;
}

//往端口写字符,cWriteData为要写字符的指针
BOOL CPort::WriteChar(unsigned char cWriteData)
{
	if (!m_bInited)
	{
		DebugMsgStr("端口尚未初始化!");
		return FALSE;
	}

	//初始化变量
	DWORD BytesSent = 0;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;	

	BOOL bResult;
	//写字符
	bResult=WriteFile(m_hComm,&cWriteData,1,&BytesSent,&m_ov);

	if (bResult)
	{
		//成功则返回
		return TRUE;
	}

	//得到错误代码
	DWORD dwError = GetLastError();

	//如果不是尚未完成则返回错误信号
	if (dwError!=ERROR_IO_PENDING)
	{
		DebugMsgStr("写字符错误！");
		return FALSE;
	}
	
	//尚未完成则等待完成
	if (!GetOverlappedResult(m_hComm,&m_ov,&BytesSent,TRUE))
	{
		//失败返回错误信号
		DebugMsgStr("写字符错误！");
		return FALSE;
	}

	//成功
	return TRUE;
}

//往端口写字符串，nNumToWrite为要写字符串的长度，pDataAddr为字符串首地址
BOOL CPort::WriteData(LPVOID pDataAddr,DWORD nNumToWrite)
{
	if (!m_bInited)
	{
		DebugMsgStr("端口尚未初始化!");
		return FALSE;
	}

	//初始化变量
	DWORD BytesSent = 0;
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;	

	BOOL bResult;
	//写字符
	bResult=WriteFile(m_hComm,pDataAddr,nNumToWrite,&BytesSent,&m_ov);

	if (bResult)
	{
		//成功则返回
		return TRUE;
	}

	//得到错误代码
	DWORD dwError = GetLastError();

	//如果不是尚未完成则返回错误信号
	if (dwError!=ERROR_IO_PENDING)
	{
		DebugMsgStr("写字符串错误！");
		return FALSE;
	}
	
	//尚未完成则等待完成
	GetOverlappedResult(m_hComm,&m_ov,&BytesSent,TRUE);
	if (BytesSent!=nNumToWrite)
	{
		//失败返回错误信号
		DebugMsgStr("没有足够的字符数!");
		return FALSE;
	}
	
	//成功
	return TRUE;
}

//从端口读一个字符,cReadData接收字符,nWait-如果缓冲中没有字符,等待时间的长度
BOOL CPort::ReadChar(unsigned char *cReadData,UINT nWait)
{
	BOOL bResult;
	DWORD dwError,dwBytesRead;
	COMSTAT comstat;

	if (!m_bInited)
	{
		DebugMsgStr("端口尚未初始化!");
		return FALSE;
	}

	//得到当前输入缓存状态
	bResult = ClearCommError(m_hComm, &dwError, &comstat);

	//如果没有数据且不等待则返回错误
	if (comstat.cbInQue==0&&nWait==0)
	{
		DebugMsgStr("CPort::ReadChar-没数据,不等了!");
		return FALSE;
	}

	//读字符
	bResult = ReadFile(m_hComm,cReadData,1,&dwBytesRead,&m_ov);

	if (!bResult)
	{
		//如果不是尚未完成则返回错误
		if (dwError=GetLastError()!=ERROR_IO_PENDING)
		{
			DebugMsgStr("CPort::ReadChar-有问题!");
			return FALSE;
		}

		if (WaitForSingleObject(m_ov.hEvent,nWait)!=WAIT_OBJECT_0)
		{
			DebugMsgStr("CPort::ReadChar-没等到(足够)数据!");
			return FALSE;
		}

	}

	//返回正确
	return TRUE;	
}

//从端口读nNumToRead个字符,存在以pDataAddr为首地址的缓存中
//m_nWait表示输入缓存中如果没有足够的数据,等待时间的长度
BOOL CPort::ReadData(LPVOID pDataAddr,DWORD nNumToRead,UINT m_nWait)
{
	BOOL bResult;
	DWORD dwError,dwBytesRead;
	COMSTAT comstat;

	if (!m_bInited)
	{
		DebugMsgStr("端口尚未初始化!");
		return FALSE;
	}

	//得到当前输入缓存状态
	bResult = ClearCommError(m_hComm, &dwError, &comstat);

	//如果没有数据且不等待则返回错误
	if (comstat.cbInQue==0&&m_nWait==0)
	{
		DebugMsgStr("CPort::ReadString-没数据,不等了!");
		return FALSE;
	}

	//读字符
	bResult = ReadFile(m_hComm,pDataAddr,nNumToRead,&dwBytesRead,&m_ov);

	if (!bResult)
	{
		//如果不是尚未完成则返回错误
		if (dwError=GetLastError()!=ERROR_IO_PENDING)
		{
			DebugMsgStr("CPort::ReadString-有问题!");
			return FALSE;
		}
	
		if (WaitForSingleObject(m_ov.hEvent,m_nWait)!=WAIT_OBJECT_0)
		{
			DebugMsgStr("CPort::ReadString-没等到(足够)数据!");
			return FALSE;
		}
			
	}

	//返回正确
	return TRUE;	

}

//从端口读一个字符,如果没有则等到有为止
BOOL CPort::WaitChar(unsigned char *pCharData)
{
	DWORD pEvtMask,dwError;
	COMSTAT comstat;
	BOOL bResult;
	unsigned char cGetData;

	//端口未初始化错误
	if (!m_bInited)
	{
		DebugMsgStr("端口尚未初始化!");
		return FALSE;
	}

	//判断输入缓存中是否已经有了字符
	bResult=ClearCommError(m_hComm, &dwError, &comstat);
	//有则读出并返回正确
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
	//等到读出一个字符为止
	while (nNum==0)
	{
		bResult=WaitCommEvent(m_hComm,&pEvtMask,&m_ov);

		if (bResult)
		{
			//先判断是否是出现了时间差引起的错误
			bResult = ClearCommError(m_hComm, &dwError, &comstat);
			//是错误则跳过这次循环,重新等待
			if (comstat.cbInQue==0)
				continue;
		}
		else
			dwError = GetLastError();

		//等待字符到来
		WaitForSingleObject(m_ov.hEvent,INFINITE);

		//读字符
		bResult=ReadChar(&cGetData);

		if (bResult)
		{
			//读出了则返回正确结果
			*pCharData=cGetData;
			nNum++;
		}
		else
		{
			//出了错误,继续下一次等待
			DebugMsgStr("WaitChar:read error!");
		}
	}
	return TRUE;
}

//清缓存
void CPort::ClearBuffer()
{
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);		
}

//返回端口状态
BOOL CPort::Inited()
{
	return m_bInited;
}

BOOL CPort::SetMark(BOOL bMark)
{
	DCB m_dcb;
	if (!GetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("试图得到DCB错误！");
		return FALSE;
	}	

	if (bMark)
		m_dcb.Parity = MARKPARITY;
	else
		m_dcb.Parity = SPACEPARITY;
	
	if (!SetCommState(m_hComm, &m_dcb))
	{
		DebugMsgStr("设置端口DCB错误！");
		return FALSE;
	}
	return TRUE;
}