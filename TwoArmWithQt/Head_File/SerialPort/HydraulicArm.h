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

	//���ص�ǰҺѹ�ۿ�����������״̬
	bool IsConnect();

	//PTP�˶�
	//angle(���ؽڽǶ�)
	//FlagRad(���ȱ�ʶ):TRUE��λΪ���ȣ�FALSEΪ����
	bool PTP(float* angle,bool FlagRad = true);

	//PTPRel�˶�
	//AngleDif(���ؽڽǶȲ�ֵ)
	//FlagRad(���ȱ�ʶ):TRUE��λΪ���ȣ�FALSEΪ����
	bool PTPRel(float* AngleDif, bool FlagRad = true);

	//���ܽ�������֡
	bool HydraulicArmAnalyse();

	//Һѹ������
	void PositiveKinematics(float* theta, Matrix4d* result);
	//Һѹ�����
	void InverseKinematics(Matrix4d& InputT, float* theta);
	//��ȡҺѹ�۵ĵ�ǰTCPλ��
	void GetTCP(float* result);

	//ֱ�߲岹
	void Lin(float dx, float dy, float dz, float step=1.0);

	//�㶯�岹
	void LinRel(float dx, float dy, float dz);

private:
	//Һѹ�۵��ؽ�����ת��
	vector<unsigned char> FloatAngleToChar(float angle);
	const float alpha[6] = { 0, -PI / 2, 0, -PI / 2, PI / 2, -PI / 2 };
	const float a[6] = { 0.0 , 72.0 , 630.0 , 125.0 , 0.0 , 0.0 };
	const float d[6] = { 290.0 , 0.0 , 0.0 , 377.0 , 0.0 , 200.0 };//D-H����
	Matrix4d TCPMatrix;
};

#endif 

