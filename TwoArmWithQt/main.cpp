#include"Login.h"
#include"ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"
#include <QtWidgets/QApplication>

//�������ͷ�ļ�
#include"ToolConnect.h"
#include"RobotArm.h"
#include <conio.h>//��������ͷ�ļ�
#include "rotation.h"

Login* login;
ModeSelect* modeselect;
DebugMode* debugmode;
WorkMode* workmode;
ServeMode* servemode;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	login = new Login(0);
	modeselect = new ModeSelect(0);
	debugmode = new DebugMode(0);
	workmode = new WorkMode(0);
	servemode = new ServeMode(0);

	RobotArm Robot;
	ToolConnect ToolConnectCom;
	ToolConnectCom.OpenCom("\\\\.\\COM16", 19200, 0, 8, 1);//�򿪹��߿��ƴ���
	Robot.StartUp();//���������߳�


	login->show();
	return a.exec();
} 


