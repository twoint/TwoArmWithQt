#include "Login.h"
#include <QtWidgets/QApplication>


//�������ͷ�ļ�
#include"ToolConnect.h"
#include"RobotArm.h"
#include <conio.h>//��������ͷ�ļ�
#include "rotation.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	RobotArm Robot;
	ToolConnect ToolConnectCom;
	ToolConnectCom.OpenCom("\\\\.\\COM16", 19200, 0, 8, 1);//�򿪹��߿��ƴ���
	Robot.StartUp();//���������߳�

	Login w;
	w.show();
	return a.exec();
}

//���Ͳ���