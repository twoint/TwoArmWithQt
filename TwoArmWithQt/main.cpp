#include"Login.h"
#include"ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"
#include <QtWidgets/QApplication>

//控制相关头文件
#include"ToolConnect.h"
#include"RobotArm.h"
#include <conio.h>//按键触发头文件
#include "rotation.h"

Login* login;
ModeSelect* modeselect;
DebugMode* debugmode;
WorkMode* workmode;
ServeMode* servemode;

RobotArm Robot;
ToolConnect ToolConnectCom;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	login = new Login(0);
	modeselect = new ModeSelect(0);
	debugmode = new DebugMode(0);
	workmode = new WorkMode(0);
	servemode = new ServeMode(0);

	ToolConnectCom.OpenCom("COM1", 19200, 0, 8, 1);//打开工具控制串口
	Robot.StartUp();//启动其它线程

	login->show();
	return a.exec();
}