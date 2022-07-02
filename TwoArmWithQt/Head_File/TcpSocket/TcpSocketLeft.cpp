#include "TcpSocketLeft.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

TcpSocketLeft::TcpSocketLeft()
{
	m_dwVserion_left = 0;
	m_isSocketFlag_left = false;
	memset(&m_WsData_left, 0, sizeof(WSADATA));
}

TcpSocketLeft::~TcpSocketLeft()
{
	closesocket(m_Scoket_left);
	WSACleanup();
}

bool TcpSocketLeft::CreatSocketLeft(string strIp, int nPort)
{
	m_isSocketFlag_left = true;
	m_dwVserion_left = MAKEWORD(1, 1);
	m_iError_left = WSAStartup(m_dwVserion_left, &m_WsData_left);
	if (m_iError_left != 0)
	{
		//cout << "socket 初始化操作失败" << endl;
		m_isSocketFlag_left = false;
	}
	if (LOBYTE(m_WsData_left.wVersion) != 1 && HIBYTE(m_WsData_left.wVersion) != 1)
	{
		WSACleanup();
		m_isSocketFlag_left = false;
	}
	if (!m_isSocketFlag_left)
		return false;
	m_Scoket_left = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(strIp.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);

	m_iError_left = connect(m_Scoket_left, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (m_iError_left != 0)
		m_isSocketFlag_left = false;
	return m_isSocketFlag_left;
}

void TcpSocketLeft::CloseSocketLeft() {
	m_isSocketFlag_left = false;
	closesocket(m_Scoket_left);
	WSACleanup();
}

bool TcpSocketLeft::RecvMsgLeft(char* RecvBuf,int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag_left)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket_left, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket_left, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = recv(m_Scoket_left, RecvBuf, len, 0);
	if (iRet == -1)
	{
		//cout << "接收服务器消息超时" << endl;
		return false;
	}
	return true;
}

bool TcpSocketLeft::SendMsgLeft(char* SendBuf, int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag_left)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket_left, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket_left, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = send(m_Scoket_left, SendBuf, len, 0);
	if (iRet == 0)
	{
		//cout << "发送服务器消息超时" << endl;
		return false;
	}
	return true;
}
