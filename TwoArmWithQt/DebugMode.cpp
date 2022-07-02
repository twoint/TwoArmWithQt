#pragma execution_character_set("utf-8")//���QLabel��������

#include "DebugMode.h"
#include"ModeSelect.h"
#include"DebugPrintf.h"
#include"RobotArm.h"
#include"ToolConnect.h"

extern RobotArm Robot;
extern ToolConnect ToolConnectCom;

static float LeftValue = 1.0f;		//���ʾ�̱���
static float RightValue = 1.0f;		//�ұ�ʾ�̱���
float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//ȥ���ڱ߿�
	//setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	infoUpdateTimer = new QTimer(this);										//��������Ϣˢ�¶�ʱ��
	infoUpdateTimer->start(33);

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout

	//connect(ui.btn_lead_gripper_on, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_on()));
	//connect(ui.btn_lead_gripper_off, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_off()));
	//connect(ui.btn_arrester_grab_on, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_on()));
	//connect(ui.btn_arrester_grab_off, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_off()));

	connect(ui.btn_left_connect, SIGNAL(clicked()), this, SLOT(on_btn_left_connect()));
	connect(ui.btn_right_connect, SIGNAL(clicked()), this, SLOT(on_btn_right_connect()));

	connect(ui.btn_left_break_open, SIGNAL(clicked()), this, SLOT(on_btn_left_break_open()));
	connect(ui.btn_left_break_close, SIGNAL(clicked()), this, SLOT(on_btn_left_break_close()));
	connect(ui.btn_right_break_open, SIGNAL(clicked()), this, SLOT(on_btn_right_break_open()));
	connect(ui.btn_right_break_close, SIGNAL(clicked()), this, SLOT(on_btn_right_break_close()));

	connect(ui.btn_left_arm_forword, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_forward()));
	connect(ui.btn_left_arm_backward, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_backward()));
	connect(ui.btn_left_arm_left, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_left()));
	connect(ui.btn_left_arm_right, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_right()));
	connect(ui.btn_left_arm_up, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_up()));
	connect(ui.btn_left_arm_down, SIGNAL(clicked()), this, SLOT(on_btn_left_arm_down()));

	connect(ui.btn_right_arm_forword, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_forward()));
	connect(ui.btn_right_arm_backward, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_backward()));
	connect(ui.btn_right_arm_left, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_left()));
	connect(ui.btn_right_arm_right, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_right()));
	connect(ui.btn_right_arm_up, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_up()));
	connect(ui.btn_right_arm_down, SIGNAL(clicked()), this, SLOT(on_btn_right_arm_down()));


}

void DebugMode::infoUpdateFrame()
{
	//���������Ϣ
	if (!Robot.LeftInf.Connect)
	{
		ui.label_left_arm_connect->setText("δ����");
		ui.label_left_arm_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_arm_connect->setText("������");
		ui.label_left_arm_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//�ұ�������Ϣ
	if (!Robot.RightInf.Connect)
	{
		ui.label_right_arm_connect->setText("δ����");
		ui.label_right_arm_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_right_arm_connect->setText("������");
		ui.label_right_arm_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//��۱�բ��Ϣ
	if (!Robot.LeftInf.Brake)
	{
		ui.label_left_arm_break->setText("��բ�ر�");
		ui.label_left_arm_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_arm_break->setText("��բ����");
		ui.label_left_arm_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	//�ұ۱�բ��Ϣ
	if (!Robot.RightInf.Brake)
	{
		ui.label_right_arm_break->setText("��բ�ر�");
		ui.label_right_arm_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_arm_break->setText("��բ����");
		ui.label_right_arm_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//�ұ۽Ƕ���Ϣ
	//ui.L_Joint_1->setValue(CurURData.Left_q_actual[1] * 180 / pi);
}

/************************��е���������ӡ���բ����******************************/
void DebugMode::on_btn_left_connect()
{
	Robot.CreatSocketLeft("192.168.1.100", 8010);
}

void DebugMode::on_btn_right_connect()
{
	Robot.CreatSocketRight("192.168.1.101", 8010);
	//Robot.OpenCom("COM1", 115200, 0, 8, 1);//��������
}

void DebugMode::on_btn_left_break_open()
{
	Robot.Set(LeftArm, OpenBrake);
}

void DebugMode::on_btn_left_break_close()
{
	Robot.Set(LeftArm, CloseBrake);
}

void DebugMode::on_btn_right_break_open()
{
	Robot.Set(RightArm, OpenBrake);
}

void DebugMode::on_btn_right_break_close()
{
	Robot.Set(RightArm, CloseBrake);
}


/************************���߿���******************************/
//void DebugMode::on_btn_lead_gripper_on()
//{
//	ToolConnectCom.command(1, 1, 0, 0);
//}
//
//void DebugMode::on_btn_lead_gripper_off()
//{
//	ToolConnectCom.command(1, 2, 0, 0);
//}
//
//void DebugMode::on_btn_arrester_grab_on()
//{
//	ToolConnectCom.command(2, 1, 0, 0);
//}
//
//void DebugMode::on_btn_arrester_grab_off()
//{
//	ToolConnectCom.command(2, 2, 0, 0);
//}

/************************�������******************************/
//��
void DebugMode::on_btn_left_arm_forward()
{
	Angle_Dis_Left[1] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_backward()
{
	Angle_Dis_Left[1] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_left()
{
	Angle_Dis_Left[0] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_right()
{
	Angle_Dis_Left[0] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_up()
{
	Angle_Dis_Left[2] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_down()
{
	Angle_Dis_Left[2] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}
//��
void DebugMode::on_btn_right_arm_forward()
{
	Angle_Dis_Right[1] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_backward()
{
	Angle_Dis_Right[1] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_left()
{
	Angle_Dis_Right[0] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_right()
{
	Angle_Dis_Right[0] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_up()
{
	Angle_Dis_Right[2] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_down()
{
	Angle_Dis_Right[2] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}


/************************�˳�*************************/
void DebugMode::on_btn_exit()
{
	emit jumpPageTo(1);
}
