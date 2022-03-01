#ifndef _ToolConnect_H
#define _ToolConnect_H

#include <iostream>
#include <windows.h>
#include <string.h>
#include "SerialPort.h"

using std::vector;

class ToolConnect :public SerialPort {
public:
	// idΪ�豸ID��powerΪ����(1ָ����������0ָ�͵���)
	// status �豸ID��0x0C(��Ƥװ��)��Ч��0x00ָ��е��ԭ�ز�����0x01ָ��е�ۺ����ƶ�
	unsigned char id;
	unsigned char power;
	unsigned char status;
	ToolConnect() {
		id = 0;
		power = 0;
		status = 0;
	}
	~ToolConnect() {};

	// ID��0x01--�綯ץ��1��0x02--�綯ץ��2��0x03--�綯���֣�0x04--�綯Һѹ����0x05--�Զ��ӻ�װ�ã�
	//     0x0B--���̽ӵ��߼�װ�ã�0x0C--��Ƥװ��
	bool command(char ID, char order1, char order2, char order3) {
		vector<unsigned char> sendbuf = { 0x31,0x00,0x00,0x00,0x00,0x00,0xff,0xff };
		sendbuf[1] = ID;
		sendbuf[2] = order1;
		sendbuf[3] = order2;
		sendbuf[4] = order3;
		if (this->send(sendbuf) != 0) return true;
		else return false;
	}

	//������������֡
	bool Analyse() {
		vector<unsigned char> recvbuf(8, 0);

		if (this->receive(&recvbuf[0], 8) == 0) return false;
		if (recvbuf[5] == 0x00 && recvbuf[6] == 0xff && recvbuf[7] == 0xff) {
			id = recvbuf[0];
			power = (recvbuf[1] == 0x48) ? 1 : 0;
			status = recvbuf[2];
			return true;
		}
		return false;
	}
};


#endif // !_ToolConnect_H
