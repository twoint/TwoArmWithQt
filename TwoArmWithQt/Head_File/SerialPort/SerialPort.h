#ifndef _SerialPort_H
#define _SerialPort_H

#include <iostream>
#include <windows.h>
#include <string.h>
#include <vector>

using std::vector;

class SerialPort
{
public:
	

	SerialPort();
	~SerialPort();

	// �򿪴���,�ɹ�����true��ʧ�ܷ���false
	// portname(������): ��Windows����"COM1""COM2"�ȣ���Linux����"/dev/ttyS1"��
	// baudrate(������): 9600��19200��38400��43000��56000��57600��115200 
	// parity(У��λ): 0Ϊ��У�飬1Ϊ��У�飬2ΪżУ�飬3Ϊ���У��
	// databit(����λ): 4-8��ͨ��Ϊ8λ
	// stopbit(ֹͣλ): 1Ϊ1λֹͣλ��2Ϊ2λֹͣλ,3Ϊ1.5λֹͣλ
	// synchronizable(ͬ�����첽): 0Ϊ�첽��1Ϊͬ��
	bool OpenCom(const char* portname, int baudrate = 115200, char parity = 0, char databit = 8, char stopbit = 1, char synchronizeflag = 0);

	//�رմ���
	void CloseCom();

protected:
	//���ӱ�ʶ
	bool IsSerialFlag;
	//�������ݻ�д���ݣ��ɹ����ط������ݳ��ȣ�ʧ�ܷ���0
	bool send(vector<unsigned char>& buf);

	//�������ݻ�����ݣ��ɹ����ض�ȡʵ�����ݵĳ��ȣ�ʧ�ܷ���0
	bool receive(unsigned char* RecvBuf, int RecvLen);

private:
	int pHandle[16];
	//ͬ����ʶ
	char synchronizeflag;
	
};

#endif
