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

class TcpSocket
{
public:
	TcpSocket();
	~TcpSocket();

	// 创建socket连接,成功返回true，失败返回false
	// strIp(服务器IP)
	// nPort(服务器端口)
	bool CreatSocket(string strIp, int nPort);
	//关闭socket连接
	void CloseSocket();

protected:
	//连接标识
	bool m_isSocketFlag;

	//发送数据：RecvBuf首地址、len发送长度
	bool RecvMsg(char* RecvBuf, int len);
	//接收数据：SendBuf首地址、len发送长度
	bool SendMsg(char* SendBuf, int len);

private:
	SOCKET m_Scoket;
	DWORD m_dwVserion;
	WSADATA m_WsData;
	int m_iError;
	
};

#endif