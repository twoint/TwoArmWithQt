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

	// ����socket����,�ɹ�����true��ʧ�ܷ���false
	// strIp(������IP)
	// nPort(�������˿�)
	bool CreatSocketLeft(string strIp, int nPort);
	//�ر�socket����
	void CloseSocketLeft();

protected:
	//���ӱ�ʶ
	bool m_isSocketFlag_left;

	//�������ݣ�RecvBuf�׵�ַ��len���ͳ���
	bool RecvMsgLeft(char* RecvBuf, int len);
	//�������ݣ�SendBuf�׵�ַ��len���ͳ���
	bool SendMsgLeft(char* SendBuf, int len);

private:
	SOCKET m_Scoket_left;
	DWORD m_dwVserion_left;
	WSADATA m_WsData_left;
	int m_iError_left;
	
};

#endif