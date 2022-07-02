#ifndef _TcpSocketRight_H
#define _TcpSocketRight_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using std::string;

class TcpSocketRight
{
public:
	TcpSocketRight();
	~TcpSocketRight();

	// 创建socket连接,成功返回true，失败返回false
	// strIp(服务器IP)
	// nPort(服务器端口)
	bool CreatSocketRight(string strIp, int nPort);
	//关闭socket连接
	void CloseSocketRight();

protected:
	//连接标识
	bool m_isSocketFlag_right;

	//发送数据：RecvBuf首地址、len发送长度
	bool RecvMsgRight(char* RecvBuf, int len);
	//接收数据：SendBuf首地址、len发送长度
	bool SendMsgRight(char* SendBuf, int len);

private:
	SOCKET m_Scoket_right;
	DWORD m_dwVserion_right;
	WSADATA m_WsData_right;
	int m_iError_right;

};

#endif