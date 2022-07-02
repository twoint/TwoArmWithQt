#ifndef _KebaConnectRight_H
#define _KebaConnectRight_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocketRight.h"

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

class KebaConnectRight :public TcpSocketRight
{
protected:
	//当前控制器模式：
	//1(手动模式)，
	//2(自动模式)，
	//3(外部自动模式)只有在这个模式下才可以开启抱闸，正常执行指令
	//4(有关节未上电)
	int pattern_right;
	//机械臂抱闸状态，true为正常打开，false为机器人关闭或报错或停止
	bool enable_right;
	//控制器脚本运行状态，true为脚本正常运行，false为脚本未开启或报错
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
	//功能指令发送，func为功能标识，可用范围为[OpenBrake,PTP_TCP)
	bool SetMotionRight(int func) {
		if ((!this->m_isSocketFlag_right) || func >= PTP_TCP || func < OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsgRight((char*)SendBuf, this->SendLen_right))return true;
		else return false;
	}
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(float型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(double型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//接收、解析数据帧，需要单独建个线程进行处理
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