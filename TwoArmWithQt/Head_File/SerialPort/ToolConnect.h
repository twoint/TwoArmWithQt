#ifndef _ToolConnect_H
#define _ToolConnect_H

#include <iostream>
#include <windows.h>
#include <string.h>
#include "SerialPort.h"

using std::vector;

class ToolConnect :public SerialPort {
public:
	// id为设备ID，power为电量(1指电量正常，0指低电量)
	// status 设备ID是0x0C(剥皮装置)有效，0x00指机械臂原地不动，0x01指机械臂横向移动
	unsigned char id;
	unsigned char power;
	unsigned char status;
	ToolConnect() {
		id = 0;
		power = 0;
		status = 0;
	}
	~ToolConnect() {};

	// ID：0x01--电动抓手1，0x02--电动抓手2，0x03--电动扳手，0x04--电动液压剪，0x05--自动接火装置，
	//     0x0B--穿刺接地线夹装置，0x0C--剥皮装置
	bool command(char ID, char order1, char order2, char order3) {
		vector<unsigned char> sendbuf = { 0x31,0x00,0x00,0x00,0x00,0x00,0xff,0xff };
		sendbuf[1] = ID;
		sendbuf[2] = order1;
		sendbuf[3] = order2;
		sendbuf[4] = order3;
		if (this->send(sendbuf) != 0) return true;
		else return false;
	}

	//解析接收数据帧
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
