#pragma once
#include<math.h>;
//#define pi 3.1415926
#include <opencv2\opencv.hpp>
#include <iostream>
#include <vector>
#include "AutoWork.h"

extern ToolConnect ToolConnectCom;
extern RobotArm Robot;


using namespace cv;
using namespace std;

Mat polyfit(std::vector<Point>& in_point, int n);

int carve()
{
	//数据输入
	Point in[19] = { Point(50,120),Point(74,110),Point(98,100),Point(122,100),Point(144,80)
		,Point(168,80),Point(192,70),Point(214,50),Point(236,40),Point(262,20)
		,Point(282,20),Point(306,30),Point(328,40),Point(356,50),Point(376,50)
		,Point(400,50),Point(424,50),Point(446,40),Point(468,30) };

	std::vector<Point> in_point(begin(in), end(in));

	//n:多项式阶次
	int n = 3;
	Mat mat_k = polyfit(in_point, n);


	//计算结果可视化
	Mat out(150, 500, CV_8UC3, Scalar::all(0));

	//画出拟合曲线
	for (int i = in[0].x; i < in[size(in) - 1].x; ++i)
	{
		Point2d ipt;
		ipt.x = i;
		ipt.y = 0;
		for (int j = 0; j < n + 1; ++j)
		{
			ipt.y += mat_k.at<double>(j, 0)*pow(i, j);
		}
		circle(out, ipt, 1, Scalar(255, 255, 255), CV_FILLED, CV_AA);
	}

	//画出原始散点
	for (int i = 0; i < size(in); ++i)
	{
		Point ipt = in[i];
		circle(out, ipt, 3, Scalar(0, 0, 255), CV_FILLED, CV_AA);
	}

	imshow("9次拟合", out);
	waitKey(0);

	return 0;
}

Mat polyfit(std::vector<Point>& in_point, int n)
{
	int size = in_point.size();
	//所求未知数个数
	int x_num = n + 1;
	//构造矩阵U和Y
	Mat mat_u(size, x_num, CV_64F);
	Mat mat_y(size, 1, CV_64F);

	for (int i = 0; i < mat_u.rows; ++i)
		for (int j = 0; j < mat_u.cols; ++j)
		{
			mat_u.at<double>(i, j) = pow(in_point[i].x, j);
		}

	for (int i = 0; i < mat_y.rows; ++i)
	{
		mat_y.at<double>(i, 0) = in_point[i].y;
	}

	//矩阵运算，获得系数矩阵K
	Mat mat_k(x_num, 1, CV_64F);
	mat_k = (mat_u.t()*mat_u).inv()*mat_u.t()*mat_y;
	std::cout << mat_k << std::endl;
	return mat_k;
}

void CreatePath()
{
	
	int res = 100;
	float radius = 1;
	float p[3] = { 0.0, 0.0, 0.0 };
	float q[3] = { 0.0, 0.0, 0.5 };
	float s[3] = { 0.1, 0.0, 0.0 };
	float t[3] = { 0.0, 0.1, 0.0 };
	float v[600]; int idx[600];
	for (int i = 1; i <= res; i++)
	{
		float a = 2 * pi*(i - 1) / res;
		v[(i - 1) * 6 + 0] = p[0] + s[0] * radius*a + t[0] * radius*a;
		v[(i - 1) * 6 + 1] = p[1] + s[1] * radius*cos(a) + t[1] * radius*sin(a);
		v[(i - 1) * 6 + 2] = p[2] + s[2] * radius*cos(a) + t[2] * radius*sin(a);
		v[(i - 1) * 6 + 3] = q[0] + s[0] * radius*a + t[0] * radius*a;
		v[(i - 1) * 6 + 4] = q[1] + s[1] * radius*cos(a) + t[1] * radius*sin(a);
		v[(i - 1) * 6 + 5] = q[2] + s[2] * radius*cos(a) + t[2] * radius*sin(a);
		idx[(i - 1) * 6 + 0] = ((i - 1) % res) * 2;
		idx[(i - 1) * 6 + 1] = ((i - 0) % res) * 2 + 1;
		idx[(i - 1) * 6 + 2] = ((i - 1) % res) * 2 + 1;
		idx[(i - 1) * 6 + 3] = ((i - 1) % res) * 2;
		idx[(i - 1) * 6 + 4] = ((i - 0) % res) * 2;
		idx[(i - 1) * 6 + 5] = ((i - 0) % res) * 2 + 1;
	}
	simxCallScriptFunction(Robot.clientID, "ControlDummy", sim_scripttype_childscript, "CreateShape", 6 * res, idx, 6 * res, v, 0, NULL, 0, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
	return;
}

void CreatePath2()
{
	int res = 100;//int res = 100;
	float radius = 1;
	float p[3] = { 0.0, -0.21442, 0.85536 };//创建的原点
	float q[3] = { 0.0, -0.21442, 0.854 };//创建的高度
	float s[3] = { 1.0, 0.0, 0.0 };//x轴分量
	float t[3] = { 0.0, 0.0, 0.0 };//y轴分量，一般设单位向量，和y轴垂直设0， { 0.0, 1.0, 0.0 };
	//float z[3] = { 0.0, 0.0, 1.0 };
	float v[600]; int idx[600];//res =100，float v[600]; int idx[600];
	for (int i = 1; i <= res; i++)
	{
		float a = -0.55*(i - 1) / res;
		v[(i - 1) * 6 + 0] = p[0] + s[0] * radius*a + t[0] * radius*a;//x1
		v[(i - 1) * 6 + 1] = p[1] + s[1] * radius*a + t[1] * radius*a;//y1
		v[(i - 1) * 6 + 2] = p[2] - 0.90912*a*a;//z1
		v[(i - 1) * 6 + 3] = q[0] + s[0] * radius*a + t[0] * radius*a;//x2 ，x2=x1
		v[(i - 1) * 6 + 4] = q[1] + s[1] * radius*a + t[1] * radius*a;//y2  ，y2=y1
		v[(i - 1) * 6 + 5] = q[2] - 0.90912*a*a;//z2
		idx[(i - 1) * 6 + 0] = ((i - 1) % res) * 2;
		idx[(i - 1) * 6 + 1] = ((i - 0) % res) * 2 + 1;
		idx[(i - 1) * 6 + 2] = ((i - 1) % res) * 2 + 1;
		idx[(i - 1) * 6 + 3] = ((i - 1) % res) * 2;
		idx[(i - 1) * 6 + 4] = ((i - 0) % res) * 2;
		idx[(i - 1) * 6 + 5] = ((i - 0) % res) * 2 + 1;
	}
	simxCallScriptFunction(Robot.clientID, "ControlDummy", sim_scripttype_childscript, "CreateShape", 6 * res, idx, 6 * res, v, 0, NULL, 0, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
	return;
}

void AddPointCloud()
{
	float* buff;
	int size = 10000 * 3;
	buff = (float*)malloc(sizeof(float) * size * 3);
	if (buff == NULL)
	{
		std::cout << "malloc failed?" << std::endl;
	}
	else
	{
		std::cout << "malloc success" << std::endl;
	}
	int i = 0;
	std::ifstream fin("C:\\Users\\Public\\Documents\\PointDataSave.txt");
	if (fin)
	{
		while (!fin.eof())
		{
			float a[3];
			fin >> a[0] >> a[1] >> a[2];
			//std::cout << a[0] << " " << a[1] << " " << a[2] << std::endl;
			buff[i * 3 + 0] = a[0];
			buff[i * 3 + 1] = a[1];
			buff[i * 3 + 2] = a[2];
			i++;
			/*if (i == 899)
				break;*/
		}
	}
	else
	{
		std::cout << "文件打开失败" << std::endl;
	}
	fin.close();

	simxCallScriptFunction(Robot.clientID, "ControlDummyLeft", sim_scripttype_childscript, "addPointCloud", 0, NULL, size, buff, 0, NULL, 0, NULL,
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot_wait);
	simxAddStatusbarMessage(Robot.clientID, "(from C++)\"point_cloud_cpp_signal\"send\r\n", simx_opmode_oneshot);

	free(buff);//释放一下内存
	std::cout << "free ed" << std::endl;


	return;
}