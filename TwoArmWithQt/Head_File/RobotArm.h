#ifndef _RobotArm_H
#define _RobotArm_H

#include <iostream>
#include "KebaConnectLeft.h"
#include "HydraulicArm.h"
#include "TouchConnect.h"
#include "VrepConnect.h"
#include"KebaConnectRight.h"
#include"CameraShow.h"


#define LeftArm      11  //左臂
#define RightArm     12  //右臂
#define FactorPosition    1.0f	
#define FactorRotation    1.0f

class RobotArm:public KebaConnectLeft,public KebaConnectRight,public HydraulicArm,public TouchConnect,public VrepConnect,public CameraShow
{
public:
	struct RobotInf{
		bool Connect;    //连接标识
		bool Brake;      //抱闸标识
		bool Script;     //脚本标识
		bool Sync;       //同步标识
		bool TELMode;     // 控制模式：1 遥操作控制 
		float Angle[6] = { 0 };     //关节角，单位是度数
		float AngleRad[6] = { 0 };  //关节角，单位是弧度
		float TCP[6] = { 0 };       //TCP位置值
	}LeftInf,RightInf;

	RobotArm();
	~RobotArm();

	//控制机械臂函数
	//FlagRobot(哪个机械臂标识)：LeftArm为左臂，RightArm为右臂
	//func(运动功能指令)可用范围为[PTP_TCP,LinRel_Angle]
	//buf(float型6个长度的首指针)
	//velA(关节角度速度)仅LeftArm PTP时有效，范围是(0,100)
	//vel(线速度)仅LeftArm Lin时有效，范围是(0,1500)
	//FlagRad(弧度标识)仅角度输入时有效，true为输入参数单位为弧度，false为输入参数单位为度数
	void Set(int FlagRobot, int func, float* buf = NULL, int velA = 90, int vel = 500, bool FlagRad = true);

	//友元线程函数，方便访问类内成员
	friend DWORD WINAPI KebaRecvLeftThread(LPVOID lpParam);
	friend DWORD WINAPI KebaRecvRightThread(LPVOID lpParam);
	//friend DWORD WINAPI HydraulicArmRecvThread(LPVOID lpParam);
	friend DWORD WINAPI TouchThread(LPVOID lpParam);
	friend DWORD WINAPI VrepThread(LPVOID lpParam);
	friend DWORD WINAPI CameraThread(LPVOID lpParam);

	//启动线程函数
	void StartUp();
private:
	HANDLE m_handle1;
	HANDLE m_handle2;
	HANDLE m_handle3;
	HANDLE m_handle4;
	HANDLE m_handle5;
};

#endif


