#pragma once
#include <math.h>
#include "chai3d.h"
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
#include <Eigen/Dense>  
//------------------------------------------------------------------------------
using namespace chai3d;
//using namespace std;
#define PI 3.1415926f



cVector3d RotationMatrixToV3(cMatrix3d rotation) //��ת����ת��ת����
{
//cout << "orientation1:" << rotation(0, 0) << rotation(1, 0) <<  rotation(2, 0) << endl;
//cout << "orientation2:" << rotation(0, 1) << rotation(1, 1) <<  rotation(2, 1) << endl;
//cout << "orientation3:" << rotation(0, 2) << rotation(1, 2) <<  rotation(2, 2) << endl;
	Eigen::Matrix3d Mat1;
	Mat1 << rotation(0, 0) , rotation(1, 0) , rotation(2, 0), rotation(0, 1) ,  rotation(1, 1) ,   rotation(2, 1), rotation(0, 2) ,  rotation(1, 2) ,  rotation(2, 2);
	
	float Thata = acos(((Mat1(0, 0) + Mat1(1, 1) + Mat1(2, 2)) - 1) *0.5);
	Eigen::Matrix3d Mat2 = (Mat1 - Mat1.transpose()) / (2 * sin(Thata));
	cVector3d OrientationV3;
	OrientationV3(0) = (Mat2(2, 1) - Mat2(1, 2))*0.5*Thata;
	OrientationV3(1) = (Mat2(0, 2) - Mat2(2, 0))*0.5*Thata;
	OrientationV3(2) = (Mat2(1, 0) - Mat2(0, 1))*0.5*Thata;
	return OrientationV3;
};

cVector3d RotationMatrixToUnitV3(cMatrix3d rotation) //��ת����ת  ��λ ��ת����
{
	Eigen::Matrix3d Mat1;
	Mat1 << rotation(0, 0), rotation(1, 0), rotation(2, 0), rotation(0, 1), rotation(1, 1), rotation(2, 1), rotation(0, 2), rotation(1, 2), rotation(2, 2);

	float Thata = acos(((Mat1(0, 0) + Mat1(1, 1) + Mat1(2, 2)) - 1) *0.5);
	Eigen::Matrix3d Mat2 = (Mat1 - Mat1.transpose()) / (2 * sin(Thata));
	cVector3d OrientationUnitV3;
	OrientationUnitV3(0) = (Mat2(2, 1) - Mat2(1, 2))*0.5;
	OrientationUnitV3(1) = (Mat2(0, 2) - Mat2(2, 0))*0.5;
	OrientationUnitV3(2) = (Mat2(1, 0) - Mat2(0, 1))*0.5;
	return OrientationUnitV3;
};

float RotationMatrixToV3Theta(cMatrix3d rotation)//��ת����ת ��ת������ת��theta
{
	Eigen::Matrix3d Mat1;
	Mat1 << rotation(0, 0), rotation(1, 0), rotation(2, 0), rotation(0, 1), rotation(1, 1), rotation(2, 1), rotation(0, 2), rotation(1, 2), rotation(2, 2);

	float Thata = acos(((Mat1(0, 0) + Mat1(1, 1) + Mat1(2, 2)) - 1) *0.5);
	return Thata;
};

cMatrix3d V3ToRotationMatrix(cVector3d Vector) //��ת����ת��ת����
{
	Eigen::Vector3d Vector3_1;
	for (int i = 0; i < 3; i++)
	{
		Vector3_1(i) = Vector(i);
	}
	float Theta = Vector3_1.norm();
	Eigen::Vector3d Vector3_2 = Vector3_1/ Theta;
	Eigen::Matrix3d	 I	=	Eigen::Matrix3d::Identity(3,3) ;
	Eigen::Matrix3d Vector3_3;
	Vector3_3 <<	0,	-Vector3_2(2), Vector3_2(1),
								Vector3_2(2),	0,	 -Vector3_2(0),
								-Vector3_2(1), Vector3_2(0),	0;
	Eigen::Matrix3d R = cos(Theta)*I + (1 - cos(Theta))*Vector3_2*(Vector3_2.transpose()) + sin(Theta)*Vector3_3;
	cMatrix3d RotationMatrix =(cMatrix3d) R.transpose();//cMatrix3d�����ռ����ת�������ʵ��ת����R��ת�ù�ϵ

	return RotationMatrix;
};
	
cVector3d RotationMatrixToEuler3(cMatrix3d rotation)
{
	//cout << "orientation1:" << rotation(0, 0) << rotation(1, 0) <<  rotation(2, 0) << endl;
	//cout << "orientation2:" << rotation(0, 1) << rotation(1, 1) <<  rotation(2, 1) << endl;
	//cout << "orientation3:" << rotation(0, 2) << rotation(1, 2) <<  rotation(2, 2) << endl;
//float alpha1 = atan2(rotation(1, 2), rotation(2, 2));//����cMatrix3d��д//��ת����תZYXŷ����
//float belta1  =  atan2(-rotation(0, 2), sqrt(rotation(1, 2)*rotation(1, 2) + rotation(2, 2)* rotation(2, 2)));
//float gama1 = atan2(rotation(0, 1), rotation(0, 0));
	float gama1 = -atan2(rotation(1, 0), rotation(0, 0));//XYZŷ���Ƕ�Ӧv-rep����̬
	float belta1 = atan2(rotation(2, 0), sqrt(rotation(0, 0)*rotation(0, 0) + rotation(1, 0)* rotation(1, 0)));
	float alpha1 = -atan2(rotation(2, 1), rotation(2, 2));
	cVector3d OrientationEuler3 = { alpha1 ,belta1,gama1 };
	return OrientationEuler3;
}
					
cMatrix3d Euler3ToRotationMatrix( cVector3d Euler3) //ZYXŷ����ת��ת����
{

	float a = Euler3(0);
	float b = Euler3(1);
	float c = Euler3(2);
	Eigen::Matrix3d R_x; Eigen::Matrix3d R_y; Eigen::Matrix3d R_z;
	R_x	<<	1,			0,					0,  
					0,			cos(a),		- sin(a),
					0,			sin(a),		cos(a);

	R_y <<	cos(b),			0,			sin(b),
					0,						1,			0,
					-sin(b),			0,			cos(b);
	R_z <<	cos(c),		- sin(c),		0,
					sin(c),		 cos(c),		0,
					0,					0,					1;
	Eigen::Matrix3d R0 = R_z * R_y*R_x;  //zyxŷ����
	cMatrix3d RotationMatrix = (cMatrix3d)R0.transpose();//cMatrix3d�����ռ����ת�������ʵ��ת����R��ת�ù�ϵ
	return RotationMatrix;
}

cVector3d VectorAxisToRotationMatrix(float nVector1[3])
{
	//float l = -(0.0961844); float m = -(-0.995233); float n = -(0.0160899);
	float l = nVector1[0];
	float m = nVector1[1];
	float n = nVector1[2];
	float axisY[3] = { l, m, n };//Y����λ����
	float axisX[3] = { m / sqrt(m*m + l * l), -l / sqrt(m*m + l * l), 0 };//X����λ����
	float VectorModulusZ = sqrt(pow(n*l, 2) + pow(m*n, 2) + pow(-l * l - m * m, 2));
	float axisZ[3] = { -n * l / VectorModulusZ, -m * n / VectorModulusZ, (l*l + m * m) / VectorModulusZ };//Z����λ����
	Eigen::Matrix3d CameraToWorld;
	CameraToWorld << axisX[0], axisX[1], axisX[2],
		axisY[0], axisY[1], axisY[2],
		axisZ[0], axisZ[1], axisZ[2];
	//CameraToWorld.transposeInPlace();
	//cMatrix3d WorldToCamera = CameraToWorld.transpose();
	cMatrix3d WorldToCamera = CameraToWorld;
	//cout << l << endl;
	//cout << m << endl;
	//cout << n << endl;
	//cout << WorldToCamera << endl;
	cVector3d rulerAngle = RotationMatrixToEuler3(WorldToCamera);
	std::cout << "��ת����תŷ�����ǣ�" << rulerAngle(0) * 180 / PI << "," << rulerAngle(1) * 180 / PI << "," << rulerAngle(2) * 180 / PI << std::endl;
	std::cout << rulerAngle << std::endl;
	//����ָ�ü�������ֱ�Ӽ���ŷ����
	/*float alpha = atan(m*n / (-l * l - m * m)); float Beta = atan(n*l / sqrt(m*m*n*n + pow((l*l + m * m), 2))); float Gamma = -acos((m*(l*l + m * m) - m * n*n) / sqrt((l*l + m * m + n * n)* (pow((l*l + m * m), 2) + m * m*n*n)));
	float pi = 3.1415926;
	std::cout << "ŷ�����ǣ�" << alpha * 180 / pi << "," << Beta * 180 / pi << "," << Gamma * 180 / pi << std::endl;*/
	return  rulerAngle;
}
