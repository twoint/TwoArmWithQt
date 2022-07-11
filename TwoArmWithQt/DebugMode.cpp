#pragma execution_character_set("utf-8")//���QLabel��������

#include "DebugMode.h"
#include"ModeSelect.h"
#include"DebugPrintf.h"
#include"RobotArm.h"
#include"ToolConnect.h"

extern RobotArm Robot;
extern ToolConnect ToolConnectCom;

static float LeftValue = 1.0f;				//���ʾ�̱���
static float RightValue = 1.0f;				//�ұ�ʾ�̱���
static float Rel_LeftRobot = 0.017f;		//��۽Ƕ�ʾ�̱���
static float Rel_RightRobot = 0.017f;		//�ұ۽Ƕ�ʾ�̱���

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//ȥ���ڱ߿�
	//setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	infoUpdateTimer = new QTimer(this);										//��������Ϣˢ�¶�ʱ��
	infoUpdateTimer->start(33);

	ui.btn_left_arm_up->setAutoRepeat(true);
	ui.btn_left_arm_up->setAutoRepeatInterval(50);
	ui.btn_left_arm_down->setAutoRepeat(true);
	ui.btn_left_arm_down->setAutoRepeatInterval(50);


	//connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(ui.btn_exit, &QPushButton::clicked, this, [=]() {
		emit jumpPageTo(1);
	});


	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout

	//connect(ui.btn_lead_gripper_on, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_on()));
	//connect(ui.btn_lead_gripper_off, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_off()));
	//connect(ui.btn_arrester_grab_on, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_on()));
	//connect(ui.btn_arrester_grab_off, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_off()));

	connect(ui.btn_left_connect, SIGNAL(clicked()), this, SLOT(on_btn_left_connect()));
	connect(ui.btn_left_disconnect, SIGNAL(clicked()), this, SLOT(on_btn_left_disconnect()));
	connect(ui.btn_right_connect, SIGNAL(clicked()), this, SLOT(on_btn_right_connect()));
	connect(ui.btn_right_disconnect, SIGNAL(clicked()), this, SLOT(on_btn_right_disconnect()));

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

	connect(ui.btn_L_joint_0_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_0_dec()));
	connect(ui.btn_L_joint_1_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_1_dec()));
	connect(ui.btn_L_joint_2_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_2_dec()));
	connect(ui.btn_L_joint_3_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_3_dec()));
	connect(ui.btn_L_joint_4_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_4_dec()));
	connect(ui.btn_L_joint_5_dec, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_5_dec()));
	connect(ui.btn_L_joint_0_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_0_inc()));
	connect(ui.btn_L_joint_1_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_1_inc()));
	connect(ui.btn_L_joint_2_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_2_inc()));
	connect(ui.btn_L_joint_3_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_3_inc()));
	connect(ui.btn_L_joint_4_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_4_inc()));
	connect(ui.btn_L_joint_5_inc, SIGNAL(clicked()), this, SLOT(on_btn_L_joint_5_inc()));

	connect(ui.btn_R_joint_0_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_0_dec()));
	connect(ui.btn_R_joint_1_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_1_dec()));
	connect(ui.btn_R_joint_2_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_2_dec()));
	connect(ui.btn_R_joint_3_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_3_dec()));
	connect(ui.btn_R_joint_4_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_4_dec()));
	connect(ui.btn_R_joint_5_dec, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_5_dec()));
	connect(ui.btn_R_joint_0_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_0_inc()));
	connect(ui.btn_R_joint_1_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_1_inc()));
	connect(ui.btn_R_joint_2_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_2_inc()));
	connect(ui.btn_R_joint_3_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_3_inc()));
	connect(ui.btn_R_joint_4_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_4_inc()));
	connect(ui.btn_R_joint_5_inc, SIGNAL(clicked()), this, SLOT(on_btn_R_joint_5_inc()));

	
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(on_btn_stop()));
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




	//��۽Ƕ���Ϣ
	ui.label_L_joint_0->setText(QString::number(Robot.LeftInf.Angle[0], 'f', 2));
	ui.label_L_joint_1->setText(QString::number(Robot.LeftInf.Angle[1], 'f', 2));
	ui.label_L_joint_2->setText(QString::number(Robot.LeftInf.Angle[2], 'f', 2));
	ui.label_L_joint_3->setText(QString::number(Robot.LeftInf.Angle[3], 'f', 2));
	ui.label_L_joint_4->setText(QString::number(Robot.LeftInf.Angle[4], 'f', 2));
	ui.label_L_joint_5->setText(QString::number(Robot.LeftInf.Angle[5], 'f', 2));
	//�ұ۽Ƕ���Ϣ
	ui.label_R_joint_0->setText(QString::number(Robot.RightInf.Angle[0], 'f', 2));
	ui.label_R_joint_1->setText(QString::number(Robot.RightInf.Angle[1], 'f', 2));
	ui.label_R_joint_2->setText(QString::number(Robot.RightInf.Angle[2], 'f', 2));
	ui.label_R_joint_3->setText(QString::number(Robot.RightInf.Angle[3], 'f', 2));
	ui.label_R_joint_4->setText(QString::number(Robot.RightInf.Angle[4], 'f', 2));
	ui.label_R_joint_5->setText(QString::number(Robot.RightInf.Angle[5], 'f', 2));

	
	
}

/************************��е���������ӡ���բ����******************************/
void DebugMode::on_btn_left_connect()
{
	Robot.CreatSocketLeft("192.168.1.100", 8010);
}

void DebugMode::on_btn_left_disconnect()
{
	Robot.CloseSocketLeft();
}

void DebugMode::on_btn_right_connect()
{
	Robot.CreatSocketRight("192.168.1.101", 8010);
	//Robot.OpenCom("COM1", 115200, 0, 8, 1);//��������
}

void DebugMode::on_btn_right_disconnect()
{
	Robot.CloseSocketRight();
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
/********************************��е�۽Ƕȿ���*******************************/
//��ۼ�
void DebugMode::on_btn_L_joint_0_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[0] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_1_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[1] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_2_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[2] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_3_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[3] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_4_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[4] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_5_dec()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[5] = -Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}
//�����
void DebugMode::on_btn_L_joint_0_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[0] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_1_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[1] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_2_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[2] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_3_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[3] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_4_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[4] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

void DebugMode::on_btn_L_joint_5_inc()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[5] = Rel_LeftRobot;
	Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
}

//�ұۼ�
void DebugMode::on_btn_R_joint_0_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[0] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_1_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[1] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_2_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[2] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_3_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[3] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_4_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[4] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_5_dec()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[5] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}
//�ұ���
void DebugMode::on_btn_R_joint_0_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[0] = -Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_1_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[1] = Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_2_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[2] = Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_3_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[3] = Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_4_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[4] = Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_R_joint_5_inc()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[5] = Rel_RightRobot;
	Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
}

void DebugMode::on_btn_stop()
{
	Robot.Set(LeftArm, StopRobot);
}


/*********************************���߿���************************************/
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

/*********************************�������************************************/
//��
void DebugMode::on_btn_left_arm_forward()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[1] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_backward()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[1] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_left()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[0] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_right()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[0] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_up()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[2] = LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}

void DebugMode::on_btn_left_arm_down()
{
	float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Left[2] = -LeftValue;
	Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
}
//��
void DebugMode::on_btn_right_arm_forward()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[1] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_backward()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[1] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_left()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[0] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_right()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[0] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_up()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[2] = RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}

void DebugMode::on_btn_right_arm_down()
{
	float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
	Angle_Dis_Right[2] = -RightValue;
	Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
}


/***********************************�˳�***************************************/
//void DebugMode::on_btn_exit()
//{
//	emit jumpPageTo(1);
//}
