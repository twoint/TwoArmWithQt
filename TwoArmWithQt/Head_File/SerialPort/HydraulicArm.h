#ifndef _HydraulicArm_H
#define _HydraulicArm_H

#include <iostream>
#include <Eigen\Dense>
#include <windows.h>
#include <string.h>
#include <vector>
#include "SerialPort.h"

#define PI 3.1415926f

using std::vector;
using Eigen::Matrix4d;

class HydraulicArm :public SerialPort {
protected:
	float JointData[6] = { 0 };
	float RadJointData[6] = { 0 };
	float TCPData[6] = { 0 };
	HydraulicArm();
	~HydraulicArm();

	//返回当前液压臂控制器的连接状态
	bool IsConnect();

	//PTP运动
	//angle(各关节角度)
	//FlagRad(弧度标识):TRUE单位为弧度，FALSE为度数
	bool PTP(float* angle,bool FlagRad = true);

	//PTPRel运动
	//AngleDif(各关节角度差值)
	//FlagRad(弧度标识):TRUE单位为弧度，FALSE为度数
	bool PTPRel(float* AngleDif, bool FlagRad = true);

	//接受解析数据帧
	bool HydraulicArmAnalyse();

	//液压臂正解
	void PositiveKinematics(float* theta, Matrix4d* result);
	//液压臂逆解
	void InverseKinematics(Matrix4d& InputT, float* theta);
	//获取液压臂的当前TCP位置
	void GetTCP(float* result);

	//直线插补
	void Lin(float dx, float dy, float dz, float step=1.0);

	//点动插补
	void LinRel(float dx, float dy, float dz);

private:
	//液压臂单关节数据转换
	vector<unsigned char> FloatAngleToChar(float angle);
	const float alpha[6] = { 0, -PI / 2, 0, -PI / 2, PI / 2, -PI / 2 };
	const float a[6] = { 0.0 , 72.0 , 630.0 , 125.0 , 0.0 , 0.0 };
	const float d[6] = { 290.0 , 0.0 , 0.0 , 377.0 , 0.0 , 200.0 };//D-H参数
	Matrix4d TCPMatrix;
};

#endif 

