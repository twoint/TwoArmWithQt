#ifndef _RobotArm_H
#define _RobotArm_H

#include <iostream>
#include "KebaConnectLeft.h"
#include "HydraulicArm.h"
#include "TouchConnect.h"
#include "VrepConnect.h"
#include"KebaConnectRight.h"
#include"CameraShow.h"


#define LeftArm      11  //���
#define RightArm     12  //�ұ�
#define FactorPosition    1.0f	
#define FactorRotation    1.0f

class RobotArm:public KebaConnectLeft,public KebaConnectRight,public HydraulicArm,public TouchConnect,public VrepConnect,public CameraShow
{
public:
	struct RobotInf{
		bool Connect;    //���ӱ�ʶ
		bool Brake;      //��բ��ʶ
		bool Script;     //�ű���ʶ
		bool Sync;       //ͬ����ʶ
		bool TELMode;     // ����ģʽ��1 ң�������� 
		float Angle[6] = { 0 };     //�ؽڽǣ���λ�Ƕ���
		float AngleRad[6] = { 0 };  //�ؽڽǣ���λ�ǻ���
		float TCP[6] = { 0 };       //TCPλ��ֵ
	}LeftInf,RightInf;

	RobotArm();
	~RobotArm();

	//���ƻ�е�ۺ���
	//FlagRobot(�ĸ���е�۱�ʶ)��LeftArmΪ��ۣ�RightArmΪ�ұ�
	//func(�˶�����ָ��)���÷�ΧΪ[PTP_TCP,LinRel_Angle]
	//buf(float��6�����ȵ���ָ��)
	//velA(�ؽڽǶ��ٶ�)��LeftArm PTPʱ��Ч����Χ��(0,100)
	//vel(���ٶ�)��LeftArm Linʱ��Ч����Χ��(0,1500)
	//FlagRad(���ȱ�ʶ)���Ƕ�����ʱ��Ч��trueΪ���������λΪ���ȣ�falseΪ���������λΪ����
	void Set(int FlagRobot, int func, float* buf = NULL, int velA = 90, int vel = 500, bool FlagRad = true);

	//��Ԫ�̺߳���������������ڳ�Ա
	friend DWORD WINAPI KebaRecvLeftThread(LPVOID lpParam);
	friend DWORD WINAPI KebaRecvRightThread(LPVOID lpParam);
	//friend DWORD WINAPI HydraulicArmRecvThread(LPVOID lpParam);
	friend DWORD WINAPI TouchThread(LPVOID lpParam);
	friend DWORD WINAPI VrepThread(LPVOID lpParam);
	friend DWORD WINAPI CameraThread(LPVOID lpParam);

	//�����̺߳���
	void StartUp();
private:
	HANDLE m_handle1;
	HANDLE m_handle2;
	HANDLE m_handle3;
	HANDLE m_handle4;
	HANDLE m_handle5;
};

#endif


