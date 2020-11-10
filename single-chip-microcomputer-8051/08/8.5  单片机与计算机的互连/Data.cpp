#include "StdAfx.h"
#include "Data.h"
#define MACHINECOMM_DATAHEAD 0x5a //数据头
#define MACHINECOMM_DATAEND 0xa5 //数据尾
#define MACHINECOMM_OK 0x11  //数据传送成功
#define MACHINECOMM_FAIL  0x88    //数据传送失败

CData::CData()
{//初始化数据
;}

CData::~CData()
{
;}

BOOL CData::InitPort(UINT portnr,UINT baud,char parity,UINT databits,UINT stopbits)
{
return m_port.InitPort(portnr,baud,parity,databits,stopbits,EV_RXCHAR,256);
}

BOOL CData::senddata(char *datastr)
{
datastr[0]=MACHINECOMM_DATAHEAD;
datastr[5]=MACHINECOMM_DATAEND;
if(!m_port.WriteData((LPVOID)datastr,6))return false; //传送失败
return true;
//char ccccc=0x55;
//for(;;)m_port.WriteChar(datastr[5]);
//if(m_port.WriteChar((unsigned char)datastr[0])
  // &&m_port.WriteChar((unsigned char)datastr[1])
  // &&m_port.WriteChar((unsigned char)datastr[2])
  // &&m_port.WriteChar((unsigned char)datastr[3])
  // &&m_port.WriteChar((unsigned char)datastr[4])
  // &&m_port.WriteChar((unsigned char)datastr[5])))return true;
//return false;
}

BOOL CData::recvdata()
{
	unsigned char retchar;
	CString ss;
//  datastr *dDatastr;
//  char* sss;
//  m_port.ReadData(sss,sizeof(*dDatastr));
//	msg(sss);
	m_port.ClearBuffer();
	if(!m_port.ReadChar(&retchar)){/*msg("get error");*/return FALSE;}//读取失败;
//	if(retchar==MACHINECOMM_FAIL){
//          ss.Format("Received Data:%d", retchar);
//          msg(ss);
//          return FALSE;//}//传送失败;
	if(retchar==MACHINECOMM_OK)return TRUE;//传送成功;
     ss.Format("Received Data:%d", retchar);
  	            msg(ss);
				return FALSE;//}//传送失败;   
}

BOOL CData::outtemperature(char *datastr)
{
for(short i_counter=1;i_counter<=10;i_counter++)
{
	if(senddata(datastr))
	{   //msg("Send OK");
		if(recvdata())return TRUE;
			
	}
}
//msg("Transfer Error!");
return FALSE;
}
