#include"../DebugMode.h"
#include "RobotArm.h"
#include"../DebugPrintf.h"
using namespace std;

extern DebugMode* debugmode;

RobotArm::RobotArm() {
	this->m_handle1 = NULL;
	this->m_handle2 = NULL;
	this->m_handle3 = NULL;
	this->m_handle4 = NULL;
	//this->m_handle5 = NULL;

	this->LeftInf.TELMode = 0;
	this->RightInf.TELMode = 0;
};

RobotArm::~RobotArm() {};

void RobotArm::Set(int FlagRobot, int func, float* buf, int velA, int vel, bool FlagRad) {
	if (FlagRobot == LeftArm) {
		if (func < PTP_TCP) this->SetMotion(func);
		else this->SetMotion(func, buf, velA, vel, FlagRad);
	}
	else if (FlagRobot == RightArm) {
		if (func < PTP_TCP) this->SetMotionRight(func);
		else this->SetMotionRight(func, buf, velA, vel, FlagRad);
	}
}

//获取keba控制器信息

DWORD WINAPI KebaRecvThread(LPVOID lpParam)
{
	RobotArm* pR = (RobotArm*)lpParam;
	//debugmode->ThreadShow("kebakeba");
	while (true) {
		pR->LeftInf.Connect = pR->m_isSocketFlag;
		if (pR->m_isSocketFlag) {
			pR->KebaAnalyse();
			pR->LeftInf.Brake = pR->enable;
			pR->LeftInf.Script = pR->script;
			for (int i = 0; i < 6; i++) {
				pR->LeftInf.Angle[i] = pR->angle[i];
				pR->LeftInf.AngleRad[i] = pR->RadAngle[i];
				pR->LeftInf.TCP[i] = pR->tcp[i];
			}
		}

	}

	return 0;
}

DWORD WINAPI KebaRecvRightThread(LPVOID lpParam)
{
	RobotArm* pR = (RobotArm*)lpParam;
	//debugmode->ThreadShow("kebakeba");
	while (true) {
		pR->RightInf.Connect = pR->m_isSocketFlag_right;
		if (pR->m_isSocketFlag_right) {
			pR->KebaAnalyseRight();
			pR->RightInf.Brake = pR->enable_right;
			pR->RightInf.Script = pR->script_right;
			for (int i = 0; i < 6; i++) {
				pR->RightInf.Angle[i] = pR->angle_right[i];
				pR->RightInf.AngleRad[i] = pR->RadAngle_right[i];
				pR->RightInf.TCP[i] = pR->tcp_right[i];
			}
		}

	}

	return 0;
}

//获取液压臂控制器信息
//DWORD WINAPI HydraulicArmRecvThread(LPVOID lpParam)
//{
//	RobotArm* pR = (RobotArm*)lpParam;
//	//debugmode->ThreadShow("hyhy");
//	while (true) {
//		pR->RightInf.Connect = pR->IsSerialFlag;
//		if (pR->IsSerialFlag) {
//			pR->HydraulicArmAnalyse();
//			for (int i = 0; i < 6; i++) {
//				pR->RightInf.Angle[i] = pR->JointData[i];
//				pR->RightInf.AngleRad[i] = pR->RadJointData[i];
//				pR->RightInf.TCP[i] = pR->TCPData[i];
//			}
//		}
//	}
//
//	return 0;
//}

//获取Touch信息
DWORD WINAPI TouchThread(LPVOID lpParam) {

	RobotArm* pR = (RobotArm*)lpParam;
	//debugmode->ThreadShow("touchtouch");

	pR->CreateDevice();

	// simulation in now running
	pR->simulationRunning = true;
	pR->simulationFinished = false;

	while (pR->simulationRunning) {
		for (int j = 0; j < pR->numHapticDevices; j++) {
			pR->UpdateHaptics(j);

			/////////////////////////////////////////////////////////////////////
			// 按键控制v-rep设置
			/////////////////////////////////////////////////////////////////////
			// the user-switch (ON = TRUE / OFF = FALSE)
			if (pR->LeftInf.TELMode || pR->RightInf.TELMode) {
				if (pR->PreTouchData[j].button0 == 0 && pR->CurTouchData[j].button0 == 1)//设置控制点和Target点位置重合，姿态和主端Touch一致
				{
					pR->firstTouchPosition[j] = pR->CurTouchData[j].Position;//按键按下瞬间的Touch位置
					Sleep(100);//确保先先执行
				}

				if (pR->PreTouchData[j].button0 == 1 && pR->CurTouchData[j].button0 == 0) {
				}

				if (pR->PreTouchData[j].button1 == 0 && pR->CurTouchData[j].button1 == 1) {
					pR->firstTouchPosition[j] = pR->CurTouchData[j].Position;//按键按下瞬间的Touch位置
					Sleep(100);//确保先先执行
				}

				if (pR->PreTouchData[j].button1 == 1 && pR->CurTouchData[j].button1 == 0) {
				}

				float SendSBuf[6] = { 0 };

				if (pR->CurTouchData[j].button0) {
					for (int i = 0; i < 3; i++) {
						SendSBuf[i] = round(FactorPosition * (pR->CurTouchData[j].Position(i) - pR->firstTouchPosition[j](i)) * 10) / 10;
						pR->firstTouchPosition[j](i) = pR->CurTouchData[j].Position(i);
						//printf("position %d：%f\n", i, SendSBuf[i]);
					}
					if (j == 0) {
						if (pR->LeftInf.Brake &&  pR->LeftInf.TELMode) {
							pR->Set(LeftArm, PTPRel_TCP, SendSBuf);
						}
					}
					else if (j == 1) {
						if (pR->RightInf.Connect&& pR->RightInf.TELMode) {
							pR->Set(RightArm, PTPRel_TCP, SendSBuf);
						}
					}
				}
				else if (pR->CurTouchData[j].button1) {
					for (int i = 0; i < 3; i++) {
						SendSBuf[i + 3] = round(FactorRotation * (pR->CurTouchData[j].Position(i) - pR->firstTouchPosition[j](i)) * 10) / 10;
						pR->firstTouchPosition[j](i) = pR->CurTouchData[j].Position(i);
						//printf("position %d：%f\n", i, SendSBuf[i]);
					}
					if (j == 0) {
						if (pR->LeftInf.Brake &&  pR->LeftInf.TELMode) {
							pR->Set(LeftArm, PTPRel_TCP, SendSBuf);
						}
					}
					else if (j == 1) {
						if (pR->RightInf.Connect && pR->RightInf.TELMode) {
							pR->Set(RightArm, PTPRel_TCP, SendSBuf);
						}
					}
				}
			}
			memcpy(&pR->PreTouchData[j], &pR->CurTouchData[j], sizeof(pR->CurTouchData[j]));//拷贝上一个循环中的数据
		}
		Sleep(50);
	}
	// exit haptics thread
	pR->simulationFinished = true;
	return 0;
}

//获取Vrep信息
DWORD WINAPI VrepThread(LPVOID lpParam) {
	RobotArm* pR = (RobotArm*)lpParam;

	while (!pR->Open_GetHandle("Vision_sensor", "UR10Left_joint", "UR10Right_joint", "UR10Left_tip", "UR10Left_target", "ControlDummyLeft", "UR10Right_tip", "UR10Right_target", "ControlDummyRight"));

	while (true) {
		pR->GetMsg();
		if (true)
		{

		}
		//pR->SetMsg(pR->LeftInf.AngleRad, pR->RightInf.AngleRad);
		Sleep(25);
	}

	return 0;
}

//启动线程函数
void RobotArm::StartUp() {
	//DP0("@@@@@@@@@@----------startup开始执行---------@@@@@@@@@@@@@\n");
	this->m_handle1 = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(KebaRecvThread), this, 0, 0);
	//this->m_handle2 = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(HydraulicArmRecvThread), this, 0, 0);
	this->m_handle2 = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(KebaRecvRightThread), this, 0, 0);
	this->m_handle3 = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(TouchThread), this, 0, 0);
	this->m_handle4 = CreateThread(NULL, 0, LPTHREAD_START_ROUTINE(VrepThread), this, 0, 0);
}
