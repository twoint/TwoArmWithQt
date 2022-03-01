#include "Login.h"
#include <QtWidgets/QApplication>


//控制相关头文件
#include"ToolConnect.h"
#include"RobotArm.h"
#include <conio.h>//按键触发头文件
#include "rotation.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	RobotArm Robot;
	ToolConnect ToolConnectCom;
	ToolConnectCom.OpenCom("\\\\.\\COM16", 19200, 0, 8, 1);//打开工具控制串口
	Robot.StartUp();//启动其它线程

	Login w;
	w.show();
	return a.exec();
}

//推送测试