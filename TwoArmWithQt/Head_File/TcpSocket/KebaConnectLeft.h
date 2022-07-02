#ifndef _KebaConnect_H
#define _KebaConnect_H

#include <SDKDDKVer.h>
#include <Winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include "TcpSocketLeft.h"

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

class KebaConnectLeft:public TcpSocketLeft
{
protected:
	//当前控制器模式：
	//1(手动模式)，
	//2(自动模式)，
	//3(外部自动模式)只有在这个模式下才可以开启抱闸，正常执行指令
	//4(有关节未上电)
	int pattern_left;
	//机械臂抱闸状态，true为正常打开，false为机器人关闭或报错或停止
	bool enable_left;
	//控制器脚本运行状态，true为脚本正常运行，false为脚本未开启或报错
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
	//功能指令发送，func为功能标识，可用范围为[OpenBrake,PTP_TCP)
	bool SetMotionLeft(int func) {
		if ((!this->m_isSocketFlag_left) || func>= PTP_TCP || func< OpenBrake) return false;
		float SendBuf[12] = { 0 };
		SendBuf[0] = func;
		SendBuf[11] = 2.56;
		if (this->SendMsgLeft((char*)SendBuf, this->SendLen_left))return true;
		else return false;
	}
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(float型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//运动指令发送
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(double型6个长度的首指针)
	//velA(关节角度速度)仅PTP时有效，范围是(0,100)
	//vel(线速度)仅Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
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
	//接收、解析数据帧，需要单独建个线程进行处理
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