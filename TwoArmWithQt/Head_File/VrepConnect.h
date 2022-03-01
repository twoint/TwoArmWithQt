#ifndef _VrepConnect_H
#define _VrepConnect_H

#include <stdlib.h> 
#include <iostream> 
#include <windows.h>
extern "C" {
#include "extApi.h"
}

#define V_REP_ADDRESS  "127.0.0.1"        //v-rep��IP��ַ,local address 127.0.0.1
#define V_REP_PORT  19997				  //v-rep��Ĭ��ͨѶ�˿�19997
#define pi 3.1415926

class VrepConnect {
public:
	int clientID;//ͨѶ�ͻ�������

	bool FlagVrepImg;
	simxUChar* vrep_image;
	simxInt resolution[2] = { 0 };

	int DummyHandle[6];//Dummy����UR10Left_tip��UR10Left_target��ControlDummyLeft��UR10Right_tip��UR10Right_target��ControlDummyRight
	int LeftJointHandle[6];
	int RightJointHandle[6];

	bool LeftControlTrigger;					//Send ControlDummy�����ݵĴ����� 0�����ͣ�1����
	bool RightControlTrigger;

	float RecvLeftJoint[6];						//�ͻ��˽���v-rep�����ݡ���е��0λ��ʼ״̬�� { 0, -pi / 2, 0, -pi / 2, 0, 0 };
	float RecvLeftTipPR[6];					//�������Tip��position and Rotation
	float RecvLeftTargetPR[6];				//�������Target��position and Rotation
	float RecvLeftContrDumPR[6];		//�������ControlDummyLeft��position and Rotation

	float RecvRightJoint[6];                //�ͻ��˽���v-rep�����ݡ���е��0λ��ʼ״̬�� { 0, -pi / 2, 0, -pi / 2, 0, 0 };
	float RecvRightTipPR[6];				//�����ұ�Tip��position and Rotation
	float RecvRightTargetPR[6];			//�����ұ�Target��position and Rotation
	float RecvRightContrDumPR[6];	//�����ұ�ControlDummyRight��position and Rotation

	VrepConnect() {//�ж��Ƿ����ӳɹ�
		FlagVrepImg = false;
		vrep_image = NULL;//

		LeftControlTrigger = false;
		RightControlTrigger = false;
		RecvLeftJoint[0] = 0;
		RecvLeftJoint[1] = -pi / 2;
		RecvLeftJoint[2] = 0;
		RecvLeftJoint[3] = -pi / 2;
		RecvLeftJoint[4] = 0;
		RecvLeftJoint[5] = 0;
		RecvRightJoint[0] = 0;
		RecvRightJoint[1] = -pi / 2;
		RecvRightJoint[2] = 0;
		RecvRightJoint[3] = -pi / 2;
		RecvRightJoint[4] = 0;
		RecvRightJoint[5] = 0;
	};
	~VrepConnect() {
		vrep_image = NULL;
	};

	//��Vrep���Ӳ���ʼ����ȡ���
	bool Open_GetHandle(const char* VisionSensor, const char* RobotLeftJoint,const char* RobotRightJoint,
		const char* RobotLeftTip,const char* RobotLeftTarget, const char* RobotLeftDummyPR, 
		const char* RobotRightTip,const char* RobotRightTarget, const char* RobotRightDummyPR,
		int port = V_REP_PORT,const char* address = V_REP_ADDRESS){
		simxFinish(-1); // just in case, close all opened connections�ر�����v-repͨѶ
		this->clientID = simxStart(address, port, true, true, -50000, 5);
		if (this->clientID == -1) return false;
		simxGetObjectHandle(this->clientID, VisionSensor, &this->Vision_sensor, simx_opmode_oneshot_wait);
		for (int i = 0; i < 6; i++)//�Ի�е�۹ؽڻ�ȡ���
		{
			std::string handleLeft = RobotLeftJoint + std::to_string(i + 1);
			const char* HandleLeft = handleLeft.c_str();
			simxGetObjectHandle(this->clientID, HandleLeft, &this->LeftJointHandle[i], simx_opmode_oneshot_wait);

			std::string handleRight = RobotRightJoint + std::to_string(i + 1);
			const char* HandleRight = handleRight.c_str();
			simxGetObjectHandle(this->clientID, HandleRight, &this->RightJointHandle[i], simx_opmode_oneshot_wait);
		}
		simxGetObjectHandle(this->clientID, RobotLeftTip, &this->DummyHandle[0], simx_opmode_oneshot_wait);
		simxGetObjectHandle(this->clientID, RobotLeftTarget, &this->DummyHandle[1], simx_opmode_oneshot_wait);
		simxGetObjectHandle(this->clientID, RobotLeftDummyPR, &this->DummyHandle[2], simx_opmode_oneshot_wait);
		simxGetObjectHandle(this->clientID, RobotRightTip, &this->DummyHandle[3], simx_opmode_oneshot_wait);
		simxGetObjectHandle(this->clientID, RobotRightTarget, &this->DummyHandle[4], simx_opmode_oneshot_wait);
		simxGetObjectHandle(this->clientID, RobotRightDummyPR, &this->DummyHandle[5], simx_opmode_oneshot_wait);
		return true;
	}

	//����Vrep��Ϣ
	//FlagVrep�Ƿ����ͼ����ʾ�ı�ʶ��trueΪ��ʾ������֮
	void GetMsg() {
		if (this->FlagVrepImg == true) {
			if (simxGetVisionSensorImage(this->clientID, this->Vision_sensor, this->resolution, &this->vrep_image, 0, simx_opmode_oneshot) != simx_return_ok) {
				printf("vrepͼ��ʧ��\n");
			}
		}
		
		for (int i = 0; i < 6; i++){
			simxGetJointPosition(this->clientID, this->LeftJointHandle[i], &this->RecvLeftJoint[i], simx_opmode_oneshot);
			simxGetJointPosition(this->clientID, this->RightJointHandle[i], &this->RecvRightJoint[i], simx_opmode_oneshot);
		}
		this->GetDummyPosAndRot(this->DummyHandle[0], this->RecvLeftTipPR);
		this->GetDummyPosAndRot(this->DummyHandle[1], this->RecvLeftTargetPR);
		this->GetDummyPosAndRot(this->DummyHandle[2], this->RecvLeftContrDumPR);
		this->GetDummyPosAndRot(this->DummyHandle[3], this->RecvRightTipPR);
		this->GetDummyPosAndRot(this->DummyHandle[4], this->RecvRightTargetPR);
		this->GetDummyPosAndRot(this->DummyHandle[5], this->RecvRightContrDumPR);
	}

	//�������ұ۹ؽڽǶ�
	//SendLeftAngle(��۹ؽڽǶȣ�float��6�����ȵ���ָ��)
	//SendRightAngle(�ұ۹ؽڽǶȣ�float��6�����ȵ���ָ��)
	void SetMsg(float* SendLeftAngle, float* SendRightAngle) {
		if (this->LeftControlTrigger== true) {
			for (int i = 0; i < 6; i++) {
				simxSetJointPosition(this->clientID, this->LeftJointHandle[i], SendLeftAngle[i], simx_opmode_oneshot);
			}
		}
		if(this->RightControlTrigger == true) {
			for (int i = 0; i < 6; i++) {
				simxSetJointPosition(this->clientID, this->RightJointHandle[i], SendRightAngle[i], simx_opmode_oneshot);
			}
		}
	}

	//���ýű�����
	int CallScriptFunctionInt(const char* functionName, int num[2]){
		int outNumCnt = 1;//����float�ĸ���
		int outInt1[1] = { 0 };
		int* outInt = outInt1;
		simxCallScriptFunction(this->clientID, "ControlDummyLeft", sim_scripttype_childscript, functionName, 2, num, 0, NULL, 0, NULL, 0, NULL,
			&outNumCnt, &outInt, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot);
		return outInt[0];
	}

	//����λ�ú���
	void SetDummyPosAndRot(int Handle, float* SendData) {
		simxSetObjectPosition(this->clientID, Handle, -1, SendData, simx_opmode_oneshot);
		simxSetObjectOrientation(this->clientID, Handle, -1, (SendData + 3), simx_opmode_oneshot);//handle
	}


private:
	int Vision_sensor;

	void GetDummyPosAndRot(int Handle, float* RecvData){
		simxGetObjectPosition(this->clientID, Handle, -1, RecvData, simx_opmode_oneshot);
		simxGetObjectOrientation(this->clientID, Handle, -1, (RecvData + 3), simx_opmode_oneshot);
	}
};

#endif
