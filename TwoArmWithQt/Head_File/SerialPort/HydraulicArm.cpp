#include "HydraulicArm.h"

using namespace std;
using Eigen::Matrix4d;

HydraulicArm::HydraulicArm() {
}

HydraulicArm::~HydraulicArm() {
}

bool HydraulicArm::IsConnect() {
	return this->IsSerialFlag;
}

bool HydraulicArm::PTP(float* angle, bool FlagRad) {
	vector<unsigned char> a(16, 0);
	a[0] = 0x8f; //指令头，固定为0x8F
	a[1] = 0x0c; //数据长度
	a[14] = 0x0c;
	for (int i = 0; i < 6; i++)
	{
		if (FlagRad == false) angle[i] = angle[i] * PI / 180;
		vector<unsigned char> num = this->FloatAngleToChar(angle[i]);
		a[i * 2 + 2] = num[0];
		a[i * 2 + 3] = num[1];
		a[14] += a[i * 2 + 2] + a[i * 2 + 3];
	}
	a[15] = 0xFF;//指令尾，固定为0xFF
	if (this->send(a) != 0) return true;
	else return false;
}

bool HydraulicArm::PTPRel(float* AngleDif, bool FlagRad) {
	vector<unsigned char> a(16, 0);
	a[0] = 0x8f; //指令头，固定为0x8F
	a[1] = 0x0c; //数据长度
	a[14] = 0x0c;
	vector<float> SendJoint(6, 0);
	for (int i = 0; i < 6; i++)
	{
		if (FlagRad == false) {
			SendJoint[i] = this->JointData[i] + AngleDif[i];
			SendJoint[i] = SendJoint[i] * PI / 180;
		}
		else {
			SendJoint[i] = this->RadJointData[i] + AngleDif[i];
		}
		vector<unsigned char> num = this->FloatAngleToChar(SendJoint[i]);
		a[i * 2 + 2] = num[0];
		a[i * 2 + 3] = num[1];
		a[14] += a[i * 2 + 2] + a[i * 2 + 3];
	}
	a[15] = 0xFF;//指令尾，固定为0xFF
	if (this->send(a) != 0) return true;
	else return false;
}

bool HydraulicArm::HydraulicArmAnalyse() {
	vector<unsigned char> recvbuf(16, 0);
	if (this->receive(&recvbuf[0], 16) == 0) return false;
	if (recvbuf[0] == 0x8e && recvbuf[15] == 0xff) {
		for (int i = 0, j = 2; j < sizeof(recvbuf) - 2; i++, j += 2){
			int data = 0;
			data = (unsigned int)recvbuf[j + 1] << 8;
			data |= (unsigned int)recvbuf[j];

			if (data > 32767){
				data = -(data - 32768);
			}
			JointData[i] = data * 1.0 / 100.0;
		}
		for (int k = 0; k < 6; k++) {
			RadJointData[k] = JointData[k] * PI / 180.0;
		}
		this->PositiveKinematics(this->RadJointData,&this->TCPMatrix);
		this->GetTCP(&this->TCPData[0]);
		return true;
	}
	return false;
}

//液压臂单关节数据转换
vector<unsigned char> HydraulicArm::FloatAngleToChar(float angle) {
	int AngleInt;
	if (angle >= 0){
		angle = angle * 180 * 100 / PI + 0.5;//
		AngleInt = (int)angle;
	}
	else{
		angle = -angle * 180 * 100 / PI + 0.5;//
		AngleInt = (int)angle;
		AngleInt = AngleInt + 32768;//负数0000 0000 0000 0000左边头一位置1，转换为十进制为32768
	}
	vector<unsigned char> AngleChar(4);
	AngleChar[0] = AngleInt >> 24;
	AngleChar[1] = AngleInt >> 16;
	AngleChar[2] = AngleInt >> 8;
	AngleChar[3] = AngleInt;
	vector<unsigned char> SendAngleChar = { AngleChar[3] ,AngleChar[2] };
	return SendAngleChar;
}

void HydraulicArm::PositiveKinematics(float* theta, Matrix4d* OutMatrix) {
	
	Matrix4d T0;
	*OutMatrix << cos(theta[0]), -sin(theta[0]), 0, a[0],
		sin(theta[0])* cos(alpha[0]), cos(theta[0])* cos(alpha[0]), -sin(alpha[0]), -d[0] * sin(alpha[0]),
		sin(theta[0])* sin(alpha[0]), cos(theta[0])* sin(alpha[0]), cos(alpha[0]), d[0] * cos(alpha[0]),
		0, 0, 0, 1;
	for (int i = 1; i < 6; i++)
	{
		T0 << cos(theta[i]), -sin(theta[i]), 0, a[i],
			sin(theta[i])* cos(alpha[i]), cos(theta[i])* cos(alpha[i]), -sin(alpha[i]), -d[i] * sin(alpha[i]),
			sin(theta[i])* sin(alpha[i]), cos(theta[i])* sin(alpha[i]), cos(alpha[i]), d[i] * cos(alpha[i]),
			0, 0, 0, 1;
		*OutMatrix = *OutMatrix * T0;
	}
}

void HydraulicArm::InverseKinematics(Matrix4d& InputT, float* theta) {
	float nx, ox, ax, px;
	float ny, oy, ay, py;
	float nz, oz, az, pz;
	nx = InputT(0, 0);
	ox = InputT(0, 1);
	ax = InputT(0, 2);
	px = InputT(0, 3);
	ny = InputT(1, 0);
	oy = InputT(1, 1);
	ay = InputT(1, 2);
	py = InputT(1, 3);
	nz = InputT(2, 0);
	oz = InputT(2, 1);
	az = InputT(2, 2);
	pz = InputT(2, 3);

	float m, n, k, A, B, D;
	float multheta3[2];//关节角3有两解
	float theta1, theta2, theta3, theta4, theta5, theta6;
	float theta23;//关节角2和3的和

	//求关节角1
	m = py - d[5] * ay;
	n = px - d[5] * ax;
	//theta1 = atan2(m, n);
	theta1 = atan(m / n);
	theta[0] = theta1;

	//求关节角3
	m = px * cos(theta1) + py * sin(theta1) - d[5] * (ax * cos(theta1) + ay * sin(theta1)) - a[1];
	n = d[5] * az - pz + d[0];
	k = (m * m + n * n - a[2] * a[2] - a[3] * a[3] - d[3] * d[3]) / (2.0 * a[2]);
	multheta3[0] = atan2(a[3], d[3]) - atan2(k, +sqrt(a[3] * a[3] + d[3] * d[3] - k * k));
	//multheta3[0] = atan(a[3]/ d[3]) - atan(k/( +sqrt(a[3] * a[3] + d[3] * d[3] - k * k)));
	multheta3[1] = atan2(a[3], d[3]) - atan2(k, -sqrt(a[3] * a[3] + d[3] * d[3] - k * k));
	theta3 = multheta3[0];
	theta[2] = theta3;

	//求关节角2
	A = ax * cos(theta1) + ay * sin(theta1);
	B = px * cos(theta1) + py * sin(theta1) - a[1];
	D = pz - d[0];
	m = (a[3] + a[2] * cos(theta3)) * (d[5] * az - D) - (d[3] - a[2] * sin(theta3)) * (B - A * d[5]);
	n = (a[3] + a[2] * cos(theta3)) * (B - A * d[5]) + (d[3] - a[2] * sin(theta3)) * (d[5] * az - D);
	theta23 = atan2(((a[3] + a[2] * cos(theta3)) * (d[5] * az - D) - (d[3] - a[2] * sin(theta3)) * (B - A * d[5])), ((a[3] + a[2] * cos(theta3)) * (B - A * d[5]) + (d[3] - a[2] * sin(theta3)) * (d[5] * az - D)));
	k = (B - A * d[5]) * (B - A * d[5]) + (d[5] * az - D) * (d[5] * az - D);
	theta2 = theta23 - theta3;
	theta[1] = theta2;

	//求关节角4
	m = ay * cos(theta1) - ax * sin(theta1);
	n = az * sin(theta23) - (ax * cos(theta1) + ay * sin(theta1)) * cos(theta23);
	theta4 = atan(m / n);
	//需判断奇异点，当theta5接近于0
	theta[3] = theta4;

	//求关节角5
	m = ax * (cos(theta1) * cos(theta4) * cos(theta23) + sin(theta1) * sin(theta4)) + ay * (sin(theta1) * cos(theta4) * cos(theta23) - cos(theta1) * sin(theta4)) - az * cos(theta4) * sin(theta23);
	n = az * cos(theta23) + (ax * cos(theta1) + ay * sin(theta1)) * sin(theta23);
	theta5 = atan(m / n);
	theta[4] = theta5;

	//求关节角6
	m = -nx * (cos(theta1) * sin(theta4) * cos(theta23) - sin(theta1) * cos(theta4)) - ny * (sin(theta1) * sin(theta4) * cos(theta23) + cos(theta1) * cos(theta4)) + nz * sin(theta4) * sin(theta23);
	n = -ox * (cos(theta1) * sin(theta4) * cos(theta23) - sin(theta1) * cos(theta4)) - oy * (sin(theta1) * sin(theta4) * cos(theta23) + cos(theta1) * cos(theta4)) + oz * sin(theta4) * sin(theta23);
	theta6 = atan2(m, n);
	theta[5] = theta6;

}

void HydraulicArm::GetTCP(float* result) {
	result[0] = (this->TCPMatrix)(0, 3);
	result[1] = (this->TCPMatrix)(1, 3);
	result[2] = (this->TCPMatrix)(2, 3);
	result[4] = asin(-(this->TCPMatrix)(2, 0));
	if (result[4] == PI / 2){
		result[3] = 0;
		result[5] = atan2((this->TCPMatrix)(0, 1), (this->TCPMatrix)(1, 1));
	}
	else if (result[4] == -PI / 2){
		result[3] = 0;
		result[5] = -atan2((this->TCPMatrix)(0, 1), (this->TCPMatrix)(1, 1));
	}
	else{
		result[3] = atan2((this->TCPMatrix)(1, 0) / cos(result[4]), (this->TCPMatrix)(0, 0) / cos(result[4]));
		result[5] = atan2((this->TCPMatrix)(2, 1) / cos(result[4]), (this->TCPMatrix)(2, 2) / cos(result[4]));
	}
}

void HydraulicArm::Lin(float dx, float dy, float dz, float step) {
	double len = sqrt(dx * dx + dy * dy + dz * dz);
	int num = round(len / step);
	if (num < 1) return;
	float SendData[6] = { 0 };

	for (int i = 0; i < num; i++) {
		(this->TCPMatrix)(0, 3) += dx / num;
		(this->TCPMatrix)(1, 3) += dy / num;
		(this->TCPMatrix)(2, 3) += dz / num;
		this->InverseKinematics(this->TCPMatrix, &SendData[0]);
		this->PTP(SendData);
		Sleep(100);
	}
}

void HydraulicArm::LinRel(float dx, float dy, float dz) {
	float SendData[6] = { 0 };
	(this->TCPMatrix)(0, 3) += dx;
	(this->TCPMatrix)(1, 3) += dy;
	(this->TCPMatrix)(2, 3) += dz;
	this->InverseKinematics(this->TCPMatrix, &SendData[0]);
	this->PTP(SendData);
}