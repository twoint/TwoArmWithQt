#ifndef _KebaConnect_H
#define _KebaConnect_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocketLeft.h"

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

class KebaConnectLeft:public TcpSocketLeft
{
protected:
	//��ǰ������ģʽ��
	//1(�ֶ�ģʽ)��
	//2(�Զ�ģʽ)��
	//3(�ⲿ�Զ�ģʽ)ֻ�������ģʽ�²ſ��Կ�����բ������ִ��ָ��
	//4(�йؽ�δ�ϵ�)
	int pattern_left;
	//��е�۱�բ״̬��trueΪ�����򿪣�falseΪ�����˹رջ򱨴��ֹͣ
	bool enable_left;
	//�������ű�����״̬��trueΪ�ű��������У�falseΪ�ű�δ�����򱨴�
	bool script_left;
	float angle_left[6] = { 0 };
	float RadAngle_left[6] = { 0 };
	float tcp_left[6] = { 0 };

	KebaConnectLeft() {
		pattern_left = 0;
		enable_left = false;
		script_left = false;
		SendLen_left = 48;
		RecvLen_left = 64;
	}
	~KebaConnectLeft() {};
	bool IsConnectLeft() {
		return this->m_isSocketFlag_left;
	}
	//����ָ��ͣ�funcΪ���ܱ�ʶ�����÷�ΧΪ[OpenBrake,PTP_TCP)
	bool SetMotionLeft(int func) {
		if ((!this->m_isSocketFlag_left) || func>= PTP_TCP || func< OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsgLeft((char*)SendBuf, this->SendLen_left))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(float��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotionLeft(int func, float* buf, int velA = 90, int vel = 500, bool FlagRad = true) {
		if ((!this->m_isSocketFlag_left) || (!this->enable_left) || (!this->script_left)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (!(func &1))) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsgLeft((char*)SendBuf, this->SendLen_left))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(double��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotionLeft(int func, double* buf, int velA = 90, int vel = 500, bool FlagRad = false) {
		if ((!this->m_isSocketFlag_left) || (!this->enable_left) || (!this->script_left)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (func & 1 == 0)) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsgLeft((char*)SendBuf, this->SendLen_left))return true;
		else return false;
	}
	//���ա���������֡����Ҫ���������߳̽��д���
	bool KebaAnalyseLeft() {
		if (!this->m_isSocketFlag_left) return false;
		float recvbuf[16] = { 0 };
		if (!this->RecvMsgLeft((char*)recvbuf, this->RecvLen_left)) {
			return false;
		}
		if (recvbuf[0] >= 10 && recvbuf[0] <= 20 && abs(recvbuf[15] - 2.56) < 0.1) {
			this->pattern_left = recvbuf[0] - 9;
			if (recvbuf[1] == 11) this->enable_left = true;
			else this->enable_left = false;
			if (recvbuf[2] == 12) this->script_left = true;
			else this->script_left = false;

			for (int i = 0; i < 6; i++) {
				this->angle_left[i] = recvbuf[i + 3];
				this->tcp_left[i] = recvbuf[i + 9];
				this->RadAngle_left[i] = this->angle_left[i] * PI / 180.0;
			}
		}
		else {
			return false;
		}
		return true;
	}
private:
	int SendLen_left;
	int RecvLen_left;
};
#endif // !_KebaConnect_H