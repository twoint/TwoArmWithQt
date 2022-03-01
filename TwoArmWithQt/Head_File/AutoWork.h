#pragma once
#include "ToolUI.h"
#include<iostream>
#include<vector>
#include<io.h>
#include<fstream>
#include<string>

extern ToolConnect ToolConnectCom;
extern RobotArm Robot;


#define ContinuousPoint 0;
#define Move 1;
#define Velocity 2;
#define MotionPlan 3;

void ContinuousPointWork(int Arm, string  func)
{
	std::string path = "C:/Users//Administrator//Desktop//test//" + func + ".txt";
	std::ifstream is(path);
	char c;
	int row = 0;
	while (is.get(c))
	{
		if (c == '\n')
		{
			row++;
		}
	}
	double  * inf[8];
	for (int i = 0; i < 8; i++)
	{
		inf[i] = new double[row];
	}
	is.close();
	std::ifstream is1(path);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			is1 >> inf[j][i];
		}
	}
	for (int i = 0; i < row; i++)
	{
		int flag = inf[0][i];
		float joint[6] = { inf[1][i],inf[2][i], inf[3][i], inf[4][i], inf[5][i], inf[6][i] };

		Robot.Set(Arm, flag, joint);
		//Trigger_in_the_same_posture(Robot.LeftInf.Angle, joint);
		//std::cout << joint[0] << " " << joint[1] << " " << joint[2] << " " << joint[3] << " " << joint[4] << " " << joint[5] << std::endl;
	}
	is1.close();
}

void MoveWork(int Arm, string  func)
{
	std::string path = "C:/Users//Administrator//Desktop//test//" + func + ".txt";
	std::ifstream is(path);
	char c;
	int row = 0;
	while (is.get(c))
	{
		if (c == '\n')
		{
			row++;
		}
	}
	double  * inf[6];
	for (int i = 0; i < 6; i++)
	{
		inf[i] = new double[row];
	}
	is.close();
	std::ifstream is1(path);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			is1 >> inf[j][i];
		}
	}
	is1.close();
	for (int i = 0; i < row; i++)
	{

		float delta[6] = { inf[0][i],inf[1][i], inf[2][i], inf[3][i], inf[4][i], inf[5][i] };
		Robot.Set(Arm, PTPRel_TCP, delta);
		//std::cout << joint[0] << " " << joint[1] << " " << joint[2] << " " << joint[3] << " " << joint[4] << " " << joint[5] << std::endl;
	}
}

void AotoWork(int Arm, string  func, int mode)//func 表示作业  mode
{
	switch (mode) {
	case 0:
		ContinuousPointWork(Arm, func);
		break;

	case 1:
		MoveWork(Arm, func);
		break;
	}

}

void MotionPlanWork(int Arm, string  func)
{
	//暂无
}
//读取txt信息并计算直线
float * ReadTxtAndCaculateLine(float inputY, std::string a)//读取txt并且计算指定y位置的xyz，欧拉角
{
	float result[6] = { 0 ,0,0,0 ,0 ,0 };
	std::ifstream fin(a);
	
	float PointAndVector[6];
	if (fin)
	{
		fin >> PointAndVector[0] >> PointAndVector[1] >> PointAndVector[2] 
			>> PointAndVector[3] >> PointAndVector[4] >> PointAndVector[5];
		float p3[3];
		p3[1] = inputY;
		float lineK = (p3[1] - PointAndVector[1]) / PointAndVector[4];//当y=-0.2时的,k的值
		p3[0] = PointAndVector[0] + lineK * (PointAndVector[3]);  
		p3[2] = PointAndVector[2] + lineK * (PointAndVector[5]);
		float l = (PointAndVector[3]); 
		float m = (PointAndVector[4]); 
		float n = (PointAndVector[5]);

		
		//xyz 欧拉角
		result[0] = p3[0] ;// zed参数 （- 0.31，0，0.07）Realsense(- 0.29,0, 0.01) 激光雷达（0.14,0,0.29)
		result[1] = p3[1] ;		// zed参数 0
		result[2] = p3[2] ;// zed参数+ 0.07

		float nVector1[3] = { l,m,n };
		cVector3d result2=VectorAxisToRotationMatrix(nVector1);
		result[3] = result2(0);
		result[4] = result2(1);
		result[5] = result2(2);/**/
	}
	else
	{
		std::cout << "文件打开失败" << std::endl;
	}
	fin.close();

	return result;
}
int  check_if_the_same_posture(float* PosAndRoa1, float* PosAndRoa2)//float 判断是否到达位置函数
{
	int result;
	float compare1[6] = { *PosAndRoa1, *(PosAndRoa1 + 1), *(PosAndRoa1 + 2), *(PosAndRoa1 + 3), *(PosAndRoa1 + 4), *(PosAndRoa1 + 5) };
	float compare2[6] = { *PosAndRoa2, *(PosAndRoa2 + 1), *(PosAndRoa2 + 2), *(PosAndRoa2 + 3), *(PosAndRoa2 + 4), *(PosAndRoa2 + 5) };

	for (int i = 0; i < 10; i++)//能否判断点的位姿是否一致？！最长4秒等待
	{
		float DeltaAngle[6];
		for (int i = 0; i < 6; i++)
		{
			DeltaAngle[i] = fabs(compare1[i] - compare2[i]);//求差值
		}
		printf("DeltaAngle%f %f %f %f %f %f \n", DeltaAngle[0], DeltaAngle[1], DeltaAngle[2], DeltaAngle[3], DeltaAngle[4], DeltaAngle[5]);

		if ((DeltaAngle[0] < 0.001) && (DeltaAngle[1] < 0.001) && (DeltaAngle[2] < 0.001) && (DeltaAngle[3] < 0.001) && (DeltaAngle[4] < 0.001) && (DeltaAngle[5] < 0.001))
		{
			result = 1;
			break;
		}
		else
		{
			Sleep(400);
			result = -1;
		}
	}
	return result;
}

template<typename T> int  Trigger_in_the_same_posture(	T *compare, T JointAngle[6])//判断是否到达位置函数
{
	int result2;
	for (int i = 0; i < 14; i++)//能否判断机械臂到指定位置？！最长8秒等待
	{
		
		float compare1[6] = { *compare, *(compare + 1), *(compare + 2), *(compare + 3), *(compare + 4), *(compare + 5) };
		if (check_if_the_same_posture(JointAngle, compare1) == 1)
		{
			Sleep(200);
			result2 = 1;
			break;
		}
		Sleep(500);
		if (i == 15)
		{
			result2 = -1;
		}
	}
	return result2;
}

//自动接火旋转上举测试
void RotationAndUpTest(float x1, float y1, float z1)
{
	//closesocket(sClient1);//关闭
	//WSACleanup();//释放资源的操作
	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;

	float* a1111 = ReadTxtAndCaculateLine(y1, "C:\\Users\\Public\\Documents\\DataSave2.txt");


	float LeftTarPosAndRot[6];
	LeftTarPosAndRot[0] = *a1111 + x1;
	LeftTarPosAndRot[1] = *(a1111 + 1);
	LeftTarPosAndRot[2] = *(a1111 + 2) + z1;
	LeftTarPosAndRot[3] = *(a1111 + 3);
	LeftTarPosAndRot[4] = *(a1111 + 4);
	LeftTarPosAndRot[5] = *(a1111 + 5);

	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LeftTarPosAndRot);
	Sleep(1000);
	std::cout <<"关节角="<< Robot.RecvLeftJoint[0] << " " << Robot.RecvLeftJoint[1] << " " << Robot.RecvLeftJoint[2] << " " << Robot.RecvLeftJoint[3] << " " << Robot.RecvLeftJoint[4] << " " << Robot.RecvLeftJoint[5] << std::endl;
	Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);

	
}
void MoveWork(string  func)
{
	std::string path = "D:/WorkSpace//programs//RemoteTeleop2Arm32//path//" + func + ".txt";
	float X[3];
	std::ifstream is(path);
	for (int j = 0; j < 3; j++)
	{
		is >> X[j];
	}
	RotationAndUpTest(X[0], X[1], X[2]);
}
//自动接火旋转上举
void RotationAndUp(float x1, float y1, float z1,float inputWorldY=0.1 )
{
	//system("E:\\WorkSpace\\programs\\zed-pcl-master\\zed-pcl-master\\build\\Release\\ZED_with_PCL.exe");

	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;
	
	float* a1111 = ReadTxtAndCaculateLine(inputWorldY, "C:\\Users\\Public\\Documents\\DataSave2.txt");

	//std::ifstream fin("C:\\Users\\Public\\Documents\\DataSave.txt");
	float LfetTarPosAndRot[6];
	LfetTarPosAndRot[0] = *a1111+x1;
	LfetTarPosAndRot[1] = *(a1111 + 1)+y1;
	LfetTarPosAndRot[2] = *(a1111 + 2)+z1;
	LfetTarPosAndRot[3] = *(a1111 + 3); 
	LfetTarPosAndRot[4] = *(a1111 + 4);
	LfetTarPosAndRot[5]=  *(a1111 + 5);
	
		for (int i = 0; i < 6; i++)
		{
			printf("LfetTarPosAndRot:%lf\n", LfetTarPosAndRot[i] * 180 / pi);
		}
		LfetTarPosAndRot[2] = LfetTarPosAndRot[2] - 0.2;//主线下方Z负方向0.2点
		Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);
			
		//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR)==1)//此处判断是堵塞函数
			{
				Sleep(1500);
				Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);

				//printf("Left Arm初始化进行中,%s\n", input.c_str());
				//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);
				printf("到达接火装置下方位置点\n");
			}

		LfetTarPosAndRot[2] = LfetTarPosAndRot[2] + 0.2;//2主线接火位置
		Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);
			//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)
			{	//直线到达movel_Angle
				Sleep(800);
				Robot.Set(LeftArm, Lin_Angle, Robot.RecvLeftJoint);

				//printf("Left Arm初始化进行中,%s\n", input.c_str());
				//Trigger_in_the_same_posture(Robot.RecvLeftJoint, Robot.LeftInf.AngleRad);

				printf("到达接火位置点\n");
			}/**/
			LfetTarPosAndRot[0] = LfetTarPosAndRot[0] + 0.01;//3主线接火位置靠紧线夹
			Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);
			//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)
			{	//直线到达movel_Angle
				Sleep(800);
				Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);

				//printf("Left Arm初始化进行中,%s\n", input.c_str());
				//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);

				printf("到达接火位置点\n");
			}

}

//自动接火松开复位
void ZIDongJieHuoFuWei(float x=0, float y=0, float z=0,float inputWorldY = 0.1)
{
	//system("E:\\WorkSpace\\programs\\zed-pcl-master\\zed-pcl-master\\build\\Release\\ZED_with_PCL.exe");

	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;
	
	ToolConnectCom.command(7, 0, 1, 0);//打开接火，会改
	Sleep(2000);

	float* a1111 = ReadTxtAndCaculateLine(inputWorldY, "C:\\Users\\Public\\Documents\\DataSave2.txt");

	//std::ifstream fin("C:\\Users\\Public\\Documents\\DataSave.txt");
	float LfetTarPosAndRot[6];
	LfetTarPosAndRot[0] = *a1111+x;
	LfetTarPosAndRot[1] = *(a1111 + 1)+y;
	LfetTarPosAndRot[2] = *(a1111 + 2)+z;
	LfetTarPosAndRot[3] = *(a1111 + 3);
	LfetTarPosAndRot[4] = *(a1111 + 4);
	LfetTarPosAndRot[5] = *(a1111 + 5);
		for (int i = 0; i < 6; i++)
		{
			printf("LfetTarPosAndRot:%lf\n", LfetTarPosAndRot[i]);
		}
		
		LfetTarPosAndRot[0] = LfetTarPosAndRot[0] +0.01;//3主线接火位置靠紧线夹
		LfetTarPosAndRot[2] = LfetTarPosAndRot[2]- 0.40;//2主线接火位置下方
		Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);
		//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)
		{	//直线到达movel_Angle
			Sleep(1500);
			Robot.Set(LeftArm, Lin_Angle, Robot.RecvLeftJoint);

			//printf("Left Arm初始化进行中,%s\n", input.c_str());
			//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);

			printf("到达接火位置下方点\n");
		}
		//KeabSendJ(-118*pi/180, -34 * pi / 180,	122 * pi / 180,	70 * pi / 180, - 96 * pi / 180,	96 * pi / 180);
		

		//Send_URCommand(input, sClient1);
		//Trigger_in_the_same_posture(URLeftPoint + i, CurURData.Left_q_actual);
			
	


}

//自动接火穿线
void ChuanxianRotationAndUpTest(float y1)
{

	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;

	float LfetTarPosAndRot[6];

	float* b1111 = ReadTxtAndCaculateLine(y1, "C:\\Users\\Public\\Documents\\DataSave.txt");
	LfetTarPosAndRot[0] = *b1111 + 0.015;
	LfetTarPosAndRot[1] = *(b1111 + 1);
	LfetTarPosAndRot[2] = *(b1111 + 2) - 0.015;
	LfetTarPosAndRot[3] = *(b1111 + 3);
	LfetTarPosAndRot[4] = *(b1111 + 4);
	LfetTarPosAndRot[5] = *(b1111 + 5);

	
	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);

	printf("穿线完毕位置点\n");
	


}

//自动接火穿线
void ChuanxianRotationAndUp(float y1)
{

	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;
	
	float LfetTarPosAndRot[6];
	
	float* b1111 = ReadTxtAndCaculateLine(y1, "C:\\Users\\Public\\Documents\\DataSave.txt");
	LfetTarPosAndRot[0] = *b1111+0.015;
	LfetTarPosAndRot[1] = *(b1111 + 1);
	LfetTarPosAndRot[2] = *(b1111 + 2) -0.015;
	LfetTarPosAndRot[3] = *(b1111 + 3);
	LfetTarPosAndRot[4] = *(b1111 + 4);
	LfetTarPosAndRot[5] = *(b1111 + 5);
		
	/*for (int i = 0; i < 6; i++)
	{
		printf("LfetTarPosAndRot:%lf\n", LfetTarPosAndRot[i] * 180 / pi);
	}*/

	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);
	//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)
	{	//直线到达movel_Angle
		Sleep(1000);
		if((y1+0.55)<=0.001)
		{
			Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);
		}
		else
		{
			Robot.Set(LeftArm, Lin_Angle, Robot.RecvLeftJoint);
		}
		//printf("Left Arm初始化进行中,%s\n", input.c_str());
		//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);

		printf("穿线完毕位置点\n");
	}/**/
	

}


//设置vrep下左臂的TCP
void SetTargetAndTipLeftPosition(float* infloat)//
{

simxCallScriptFunction(Robot.clientID, "ControlDummyLeft", sim_scripttype_childscript, "SetTargetAndTipLeftPosition", 0, NULL, 6, infloat, 0, NULL, 0, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, simx_opmode_oneshot);

}


//故障指示器的转上举测试
void GuZhangZhiShiQiZhongJianDian()
{
	//closesocket(sClient1);//关闭
	//WSACleanup();//释放资源的操作
	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;

	float* a1111 = ReadTxtAndCaculateLine(-0.2, "C:\\Users\\Public\\Documents\\DataSave2.txt");

	//std::ifstream fin("C:\\Users\\Public\\Documents\\DataSave.txt");
	float LfetTarPosAndRot[6];
	LfetTarPosAndRot[0] = *a1111;
	LfetTarPosAndRot[1] = *(a1111 + 1);
	LfetTarPosAndRot[2] = *(a1111 + 2);
	LfetTarPosAndRot[3] = *(a1111 + 3);
	LfetTarPosAndRot[4] = *(a1111 + 4);
	LfetTarPosAndRot[5] = *(a1111 + 5);

	for (int i = 0; i < 6; i++)
	{
		printf("LfetTarPosAndRot:%lf\n", LfetTarPosAndRot[i] * 180 / pi);
	}
	LfetTarPosAndRot[0] = LfetTarPosAndRot[0] + 0.01;//斜侧方
	LfetTarPosAndRot[2] = LfetTarPosAndRot[2] - 0.2;//主线下方Z负方向0.1点
	
	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);

	//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)//此处判断是堵塞函数
	{
		Sleep(1800);
		//std::string input = movej_Angle(CurVrepData.RecvLeftJoint[0], CurVrepData.RecvLeftJoint[1], CurVrepData.RecvLeftJoint[2], CurVrepData.RecvLeftJoint[3], CurVrepData.RecvLeftJoint[4], CurVrepData.RecvLeftJoint[5], 0.5, 0.5);
		Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);
		//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);
		Sleep(4500);
		printf("到达接火装置下方位置点\n");
	}
	LfetTarPosAndRot[0] = LfetTarPosAndRot[0] + 0.01;//斜侧方
	LfetTarPosAndRot[2] = LfetTarPosAndRot[2]+ 0.2;//主线下方Z负方向0.1点
	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LfetTarPosAndRot);

	//if (check_if_the_same_posture(CurVrepData.RecvLeftTipPR, CurVrepData.RecvLeftTargetPR) == 1)//此处判断是堵塞函数
	{
		Sleep(1800);
		Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);
		//Trigger_in_the_same_posture(CurVrepData.RecvLeftJoint, CurURData.Left_q_actual);
		//Sleep(4500);
		printf("到达\n");
	}
	


}

//故障指示器的转上举测试
void GuZhangZhiShiQiRotationAndUpTest(float x1, float y1, float z1)
{
	//closesocket(sClient1);//关闭
	//WSACleanup();//释放资源的操作
	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;

	float* a1111 = ReadTxtAndCaculateLine(y1, "C:\\Users\\Public\\Documents\\DataSave2.txt");


	float LeftTarPosAndRot[6];
	LeftTarPosAndRot[0] = *a1111 + x1;
	LeftTarPosAndRot[1] = *(a1111 + 1);
	LeftTarPosAndRot[2] = *(a1111 + 2) + z1;
	LeftTarPosAndRot[3] = *(a1111 + 3);
	LeftTarPosAndRot[4] = *(a1111 + 4);
	LeftTarPosAndRot[5] = *(a1111 + 5);

	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LeftTarPosAndRot);
	Sleep(1000);
	std::cout << "关节角=" << Robot.RecvLeftJoint[0] << " " << Robot.RecvLeftJoint[1] << " " << Robot.RecvLeftJoint[2] << " " << Robot.RecvLeftJoint[3] << " " << Robot.RecvLeftJoint[4] << " " << Robot.RecvLeftJoint[5] << std::endl;
	//Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);
}


//故障指示器的转上举
void GuZhangZhiShiQiRotationAndUp(float x1, float y1, float z1)
{
	//closesocket(sClient1);//关闭
	//WSACleanup();//释放资源的操作
	//关闭所有V-rep和实体机械臂连接
	Robot.LeftInf.TELMode = 0;
	Robot.RightInf.TELMode = 0;
	Robot.LeftInf.Sync = 0;
	Robot.RightInf.Sync = 0;
	Robot.LeftControlTrigger == 0;
	Robot.RightControlTrigger == 0;

	float* a1111 = ReadTxtAndCaculateLine(y1, "C:\\Users\\Public\\Documents\\DataSave2.txt");


	float LeftTarPosAndRot[6];
	LeftTarPosAndRot[0] = *a1111 + x1;
	LeftTarPosAndRot[1] = *(a1111 + 1);
	LeftTarPosAndRot[2] = *(a1111 + 2) + z1;
	LeftTarPosAndRot[3] = *(a1111 + 3);
	LeftTarPosAndRot[4] = *(a1111 + 4);
	LeftTarPosAndRot[5] = *(a1111 + 5);

	Robot.SetDummyPosAndRot(Robot.DummyHandle[1], LeftTarPosAndRot);
	Sleep(1000);
	std::cout << "关节角=" << Robot.RecvLeftJoint[0] << " " << Robot.RecvLeftJoint[1] << " " << Robot.RecvLeftJoint[2] << " " << Robot.RecvLeftJoint[3] << " " << Robot.RecvLeftJoint[4] << " " << Robot.RecvLeftJoint[5] << std::endl;
	Robot.Set(LeftArm, PTP_Angle, Robot.RecvLeftJoint);
}