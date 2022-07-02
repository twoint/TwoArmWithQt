#ifndef _KebaConnectRight_H
#define _KebaConnectRight_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocketRight.h"

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

class KebaConnectRight :public TcpSocketRight
{
protected:
	//��ǰ������ģʽ��
	//1(�ֶ�ģʽ)��
	//2(�Զ�ģʽ)��
	//3(�ⲿ�Զ�ģʽ)ֻ�������ģʽ�²ſ��Կ�����բ������ִ��ָ��
	//4(�йؽ�δ�ϵ�)
	int pattern_right;
	//��е�۱�բ״̬��trueΪ�����򿪣�falseΪ�����˹رջ򱨴��ֹͣ
	bool enable_right;
	//�������ű�����״̬��trueΪ�ű��������У�falseΪ�ű�δ�����򱨴�
	bool script_right;
	float angle_right[6] = { 0 };
	float RadAngle_right[6] = { 0 };
	float tcp_right[6] = { 0 };

	KebaConnectRight() {
		pattern_right = 0;
		enable_right = false;
		script_right = false;
		SendLen_right = 48;
		RecvLen_right = 64;
	}
	~KebaConnectRight() {};
	bool IsConnect_right() {
		return this->m_isSocketFlag_right;
	}
	//����ָ��ͣ�funcΪ���ܱ�ʶ�����÷�ΧΪ[OpenBrake,PTP_TCP)
	bool SetMotionRight(int func) {
		if ((!this->m_isSocketFlag_right) || func >= PTP_TCP || func < OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsgRight((char*)SendBuf, this->SendLen_right))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(float��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotionRight(int func, float* buf, int velA = 90, int vel = 500, bool FlagRad = true) {
		if ((!this->m_isSocketFlag_right) || (!this->enable_right) || (!this->script_right)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (!(func & 1))) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsgRight((char*)SendBuf, this->SendLen_right))return true;
		else return false;
	}
	//�˶�ָ���
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(double��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	bool SetMotionRight(int func, double* buf, int velA = 90, int vel = 500, bool FlagRad = false) {
		if ((!this->m_isSocketFlag_right) || (!this->enable_right) || (!this->script_right)) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		for (int i = 0; i < 6; i++) {
			if (FlagRad == true && (func & 1 == 0)) SendBuf[i + 1] = buf[i] * 180 / PI;
			else SendBuf[i + 1] = buf[i];
		}
		SendBuf[7] = velA;
		SendBuf[8] = vel;
		if (this->SendMsgRight((char*)SendBuf, this->SendLen_right))return true;
		else return false;
	}
	//���ա���������֡����Ҫ���������߳̽��д���
	bool KebaAnalyseRight() {
		if (!this->m_isSocketFlag_right) return false;
		float recvbuf[16] = { 0 };
		if (!this->RecvMsgRight((char*)recvbuf, this->RecvLen_right)) {
			return false;
		}
		if (recvbuf[0] >= 10 && recvbuf[0] <= 20 && abs(recvbuf[15] - 2.56) < 0.1) {
			this->pattern_right = recvbuf[0] - 9;
			if (recvbuf[1] == 11) this->enable_right = true;
			else this->enable_right = false;
			if (recvbuf[2] == 12) this->script_right = true;
			else this->script_right = false;

			for (int i = 0; i < 6; i++) {
				this->angle_right[i] = recvbuf[i + 3];
				this->tcp_right[i] = recvbuf[i + 9];
				this->RadAngle_right[i] = this->angle_right[i] * PI / 180.0;
			}
		}
		else {
			return false;
		}
		return true;
	}
private:
	int SendLen_right;
	int RecvLen_right;
};
#endif // !_KebaConnectRight_H