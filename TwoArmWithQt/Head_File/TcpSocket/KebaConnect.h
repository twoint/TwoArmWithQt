#ifndef _KebaConnect_H
#define _KebaConnect_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocket.h"

#define PI 3.1415926f
#define OpenBrake     101  //打开抱闸
#define CloseBrake    102  //关闭抱闸
#define RestartScrip  104  //重启脚本
#define StopRobot     110  //停止机器人
//#define GetTool1      111  //获取工具1
//#define BackTool1     112  //返回工具1
//#define GetTool2      113  //获取工具2
//#define BackTool2     114  //返回工具2
#define PTP_TCP       201  //PTP运动，输入为TCP位置
#define PTP_Angle     202  //PTP运动，输入为关节角度
#define PTPRel_TCP    203  //PTP运动，输入为TCP位置差值
#define PTPRel_Angle  204  //PTP运动，输入为关节角度差值
#define Lin_TCP       205  //Lin运动，输入为TCP位置
#define Lin_Angle     206  //Lin运动，输入为关节角度
#define LinRel_TCP    207  //Lin运动，输入为TCP位置差值
#define LinRel_Angle  208  //Lin运动，输入为关节角度差值

class KebaConnect:public TcpSocket
{
protected:
	//当前控制器模式：
	//1(手动模式)，
	//2(自动模式)，
	//3(外部自动模式)只有在这个模式下才可以开启抱闸，正常执行指令
	//4(有关节未上电)
	int pattern;
	//机械臂抱闸状态，true为正常打开，false为机器人关闭或报错或停止
	bool enable;
	//控制器脚本运行状态，true为脚本正常运行，false为脚本未开启或报错
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
	//功能指令发送，func为功能标识，可用范围为[OpenBrake,PTP_TCP)
	bool SetMotion(int func) {
		if ((!this->m_isSocketFlag) || func>= PTP_TCP || func< OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsg((char*)SendBuf, this->SendLen))return true;
		else return false;
	}
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(float型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(double型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//接收、解析数据帧，需要单独建个线程进行处理
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