#ifndef _KebaConnect_H
#define _KebaConnect_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocket.h"

#define PI 3.1415926f
#define OpenBrake     101  //�򿪱�բ
#define CloseBrake    102  //�رձ�բ
#define RestartScrip  104  //�����ű�
#define StopRobot     110  //ֹͣ������
//#define GetTool1      111  //��ȡ����1
//#define BackTool1     112  //���ع���1
//#define GetTool2      113  //��ȡ����2
//#define BackTool2     114  //���ع���2
#define PTP_TCP       201  //PTP�˶�������ΪTCPλ��
#define PTP_Angle     202  //PTP�˶�������Ϊ�ؽڽǶ�
#define PTPRel_TCP    203  //PTP�˶�������ΪTCPλ�ò�ֵ
#define PTPRel_Angle  204  //PTP�˶�������Ϊ�ؽڽǶȲ�ֵ
#define Lin_TCP       205  //Lin�˶�������ΪTCPλ��
#define Lin_Angle     206  //Lin�˶�������Ϊ�ؽڽǶ�
#define LinRel_TCP    207  //Lin�˶�������ΪTCPλ�ò�ֵ
#define LinRel_Angle  208  //Lin�˶�������Ϊ�ؽڽǶȲ�ֵ

class KebaConnect:public TcpSocket
{
protected:
	//��ǰ������ģʽ��
	//1(�ֶ�ģʽ)��
	//2(�Զ�ģʽ)��
	//3(�ⲿ�Զ�ģʽ)ֻ�������ģʽ�²ſ��Կ�����բ������ִ��ָ��
	//4(�йؽ�δ�ϵ�)
	int pattern;
	//��е�۱�բ״̬��trueΪ�����򿪣�falseΪ�����˹رջ򱨴��ֹͣ
	bool enable;
	//�������ű�����״̬��trueΪ�ű��������У�falseΪ�ű�δ�����򱨴�
	bool script;
	float angle[6] = { 0 };
	float RadAngle[6] = { 0 };
	float tcp[6] = { 0 };

	KebaConnect() {
		pattern = 0;
		enable = false;
		script = false;
		SendLen = 48;
		RecvLen = 64;
	}
	~KebaConnect() {};
	bool IsConnect() {
		return this->m_isSocketFlag;
	}
	//����ָ��ͣ�funcΪ���ܱ�ʶ�����÷�ΧΪ[OpenBrake,PTP_TCP)
	bool SetMotion(int func) {
		if ((!this->m_isSocketFlag) || func>= PTP_TCP || func< OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsg((char*)SendBuf, this->SendLen))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(float��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotion(int func, float* buf, int velA = 90, int vel = 500, bool FlagRad = true) {
		if ((!this->m_isSocketFlag) || (!this->enable) || (!this->script)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (!(func &1))) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsg((char*)SendBuf, this->SendLen))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(double��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotion(int func, double* buf, int velA = 90, int vel = 500, bool FlagRad = false) {
		if ((!this->m_isSocketFlag) || (!this->enable) || (!this->script)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (func & 1 == 0)) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsg((char*)SendBuf, this->SendLen))return true;
		else return false;
	}
	//���ա���������֡����Ҫ���������߳̽��д���
	bool KebaAnalyse() {
		if (!this->m_isSocketFlag) return false;
		float recvbuf[16] = { 0 };
		if (!this->RecvMsg((char*)recvbuf, this->RecvLen)) {
			return false;
		}
		if (recvbuf[0] >= 10 && recvbuf[0] <= 20 && abs(recvbuf[15] - 2.56) < 0.1) {
			this->pattern = recvbuf[0] - 9;
			if (recvbuf[1] == 11) this->enable = true;
			else this->enable = false;
			if (recvbuf[2] == 12) this->script = true;
			else this->script = false;

			for (int i = 0; i < 6; i++) {
				this->angle[i] = recvbuf[i + 3];
				this->tcp[i] = recvbuf[i + 9];
				this->RadAngle[i] = this->angle[i] * PI / 180.0;
			}
		}
		else {
			return false;
		}
		return true;
	}
private:
	int SendLen;
	int RecvLen;
};
#endif // !_KebaConnect_H