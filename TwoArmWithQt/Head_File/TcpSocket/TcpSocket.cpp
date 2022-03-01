#include "TcpSocket.h"
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

TcpSocket::TcpSocket()
{
	m_dwVserion = 0;
	m_isSocketFlag = false;
	memset(&m_WsData, 0, sizeof(WSADATA));
}

TcpSocket::~TcpSocket()
{
	closesocket(m_Scoket);
	WSACleanup();
}

bool TcpSocket::CreatSocket(string strIp, int nPort)
{
	m_isSocketFlag = true;
	m_dwVserion = MAKEWORD(1, 1);
	m_iError = WSAStartup(m_dwVserion, &m_WsData);
	if (m_iError != 0)
	{
		//cout << "socket 初始化操作失败" << endl;
		m_isSocketFlag = false;
	}
	if (LOBYTE(m_WsData.wVersion) != 1 && HIBYTE(m_WsData.wVersion) != 1)
	{
		WSACleanup();
		m_isSocketFlag = false;
	}
	if (!m_isSocketFlag)
		return false;
	m_Scoket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr;
	addr.sin_addr.S_un.S_addr = inet_addr(strIp.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(nPort);

	m_iError = connect(m_Scoket, (SOCKADDR*)&addr, sizeof(SOCKADDR));
	if (m_iError != 0)
		m_isSocketFlag = false;
	return m_isSocketFlag;
}

void TcpSocket::CloseSocket() {
	m_isSocketFlag = false;
	closesocket(m_Scoket);
	WSACleanup();
}

bool TcpSocket::RecvMsg(char* RecvBuf,int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = recv(m_Scoket, RecvBuf, len, 0);
	if (iRet == -1)
	{
		//cout << "接收服务器消息超时" << endl;
		return false;
	}
	return true;
}

bool TcpSocket::SendMsg(char* SendBuf, int len)
{
	int iTimeOut = 5;
	if (!m_isSocketFlag)
		return false;
	int timeOut = iTimeOut * 1000;        //秒
	setsockopt(m_Scoket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeOut, sizeof(timeOut));
	setsockopt(m_Scoket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeOut, sizeof(timeOut));
	int iRet = send(m_Scoket, SendBuf, len, 0);
	if (iRet == 0)
	{
		//cout << "发送服务器消息超时" << endl;
		return false;
	}
	return true;
}
