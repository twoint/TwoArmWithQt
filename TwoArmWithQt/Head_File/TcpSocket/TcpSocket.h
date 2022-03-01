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

	// ����socket����,�ɹ�����true��ʧ�ܷ���false
	// strIp(������IP)
	// nPort(�������˿�)
	bool CreatSocket(string strIp, int nPort);
	//�ر�socket����
	void CloseSocket();

protected:
	//���ӱ�ʶ
	bool m_isSocketFlag;

	//�������ݣ�RecvBuf�׵�ַ��len���ͳ���
	bool RecvMsg(char* RecvBuf, int len);
	//�������ݣ�SendBuf�׵�ַ��len���ͳ���
	bool SendMsg(char* SendBuf, int len);

private:
	SOCKET m_Scoket;
	DWORD m_dwVserion;
	WSADATA m_WsData;
	int m_iError;
	
};

#endif