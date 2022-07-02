#include "TcpSocketRight.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

TcpSocketRight::TcpSocketRight()
{
	m_dwVserion_right = 0;
	m_isSocketFlag_right = false;
	memset(&m_WsData_right, 0, sizeof(WSADATA));
}

TcpSocketRight::~TcpSocketRight()
{
	closesocket(m_Scoket_right);
	WSACleanup();
}

bool TcpSocketRight::CreatSocketRight(string strIp, int nPort)
{
	m_isSocketFlag_right = true;
	m_dwVserion_right = MAKEWORD(1, 1);
	m_iError_right = WSAStartup(m_dwVserion_right, &m_WsData_right);
	if (m_iError_right != 0)
	{
		//cout << "socket 初始化操作失败" << endl;
		m_isSocketFlag_right = false;
	}
	if (LOBYTE(m_WsData_right.wVersion) != 1 && HIBYTE(m_WsData_right.wVersion) != 1)
	{
		WSACleanup();
		m_isSocketFlag_right = false;
	}
	if (!m_isSocketFlag_right)
		return false;
	m_Scoket_right = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(strIp.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);

	m_iError_right = connect(m_Scoket_right, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (m_iError_right != 0)
		m_isSocketFlag_right = false;
	return m_isSocketFlag_right;
}

void TcpSocketRight::CloseSocketRight() {
	m_isSocketFlag_right = false;
	closesocket(m_Scoket_right);
	WSACleanup();
}

bool TcpSocketRight::RecvMsgRight(char* RecvBuf, int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag_right)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket_right, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket_right, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = recv(m_Scoket_right, RecvBuf, len, 0);
	if (iRet == -1)
	{
		//cout << "接收服务器消息超时" << endl;
		return false;
	}
	return true;
}

bool TcpSocketRight::SendMsgRight(char* SendBuf, int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag_right)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket_right, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket_right, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = send(m_Scoket_right, SendBuf, len, 0);
	if (iRet == 0)
	{
		//cout << "发送服务器消息超时" << endl;
		return false;
	}
	return true;
}
