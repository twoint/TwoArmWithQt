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

	// ����socket����,�ɹ�����true��ʧ�ܷ���false
	// strIp(������IP)
	// nPort(�������˿�)
	bool CreatSocketRight(string strIp, int nPort);
	//�ر�socket����
	void CloseSocketRight();

protected:
	//���ӱ�ʶ
	bool m_isSocketFlag_right;

	//�������ݣ�RecvBuf�׵�ַ��len���ͳ���
	bool RecvMsgRight(char* RecvBuf, int len);
	//�������ݣ�SendBuf�׵�ַ��len���ͳ���
	bool SendMsgRight(char* SendBuf, int len);

private:
	SOCKET m_Scoket_right;
	DWORD m_dwVserion_right;
	WSADATA m_WsData_right;
	int m_iError_right;

};

#endif