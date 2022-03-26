#include <QtWidgets/QApplication>
#include"PageContainer.h"

//控制相关头文件
#include"ToolConnect.h"
#include"RobotArm.h"
#include <conio.h>//按键触发头文件
#include "rotation.h"

RobotArm Robot;
ToolConnect ToolConnectCom;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	PageContainer *pagecontainer = new PageContainer(0);

	ToolConnectCom.OpenCom("COM1", 19200, 0, 8, 1);//打开工具控制串口
	Robot.StartUp();//启动其它线程

	pagecontainer->show();
	return a.exec();
}