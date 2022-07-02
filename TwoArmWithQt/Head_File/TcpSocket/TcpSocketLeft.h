#ifndef _TcpSocket_H
#define _TcpSocket_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using std::string;

class TcpSocketLeft
{
public:
	TcpSocketLeft();
	~TcpSocketLeft();

	// 创建socket连接,成功返回true，失败返回false
	// strIp(服务器IP)
	// nPort(服务器端口)
	bool CreatSocketLeft(string strIp, int nPort);
	//关闭socket连接
	void CloseSocketLeft();

protected:
	//连接标识
	bool m_isSocketFlag_left;

	//发送数据：RecvBuf首地址、len发送长度
	bool RecvMsgLeft(char* RecvBuf, int len);
	//接收数据：SendBuf首地址、len发送长度
	bool SendMsgLeft(char* SendBuf, int len);

private:
	SOCKET m_Scoket_left;
	DWORD m_dwVserion_left;
	WSADATA m_WsData_left;
	int m_iError_left;
	
};

#endif