#pragma execution_character_set("utf-8")//���QLabel��������

#include "DebugMode.h"
#include"ModeSelect.h"
#include"DebugPrintf.h"
#include"RobotArm.h"
#include"ToolConnect.h"

extern RobotArm Robot;
extern ToolConnect ToolConnectCom;

static float left_direction_scale = 0.5f;	//��۷���ʾ�̱���
static float right_direction_scale = 0.5f;	//�ұ۷���ʾ�̱���                                
static float left_angle_scale = 0.02f;		//��۽Ƕ�ʾ�̱���
static float right_angle_scale = 0.02f;		//�ұ۽Ƕ�ʾ�̱���

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//ȥ���ڱ߿�
	//setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	infoUpdateTimer = new QTimer(this);										//��������Ϣˢ�¶�ʱ��
	infoUpdateTimer->start(33);


	ui.left_direction_slider->setValue(int(left_direction_scale * 100));
	ui.left_direction_value->setText(QString::number(left_direction_scale, 'f', 2));
	ui.left_angle_slider->setValue(int(left_angle_scale * 1000));
	ui.left_angle_value->setText(QString::number(left_angle_scale, 'f', 3));
	ui.right_direction_slider->setValue(int(right_direction_scale * 100));
	ui.right_direction_value->setText(QString::number(right_direction_scale, 'f', 2));
	ui.right_angle_slider->setValue(int(right_angle_scale * 1000));
	ui.right_angle_value->setText(QString::number(right_angle_scale, 'f', 3));


	//��Ϣˢ��
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout
	//�˳�
	connect(ui.btn_exit, &QPushButton::clicked, this, [=]() {
		emit jumpPageTo(1);
	});
	//�������
	connect(ui.btn_left_connect, &QPushButton::clicked, this, [=]() {
		Robot.CreatSocketLeft("192.168.1.100", 8010);
	});
	//��۶Ͽ�����
	connect(ui.btn_left_disconnect, &QPushButton::clicked, this, [=]() {
		Robot.CloseSocketLeft();
	});
	//�ұ�����
	connect(ui.btn_right_connect, &QPushButton::clicked, this, [=]() {
		Robot.CreatSocketRight("192.168.1.101", 8010);
	});
	//�ұ۶Ͽ�����
	connect(ui.btn_right_disconnect, &QPushButton::clicked, this, [=]() {
		Robot.CloseSocketRight();
	});
	//��۴򿪱�բ
	connect(ui.btn_left_break_open, &QPushButton::clicked, this, [=]() {
		Robot.Set(LeftArm, OpenBrake);
	});
	//��۹رձ�բ
	connect(ui.btn_left_break_close, &QPushButton::clicked, this, [=]() {
		Robot.Set(LeftArm, CloseBrake);
	});
	//�ұ۴򿪱�բ
	connect(ui.btn_right_break_open, &QPushButton::clicked, this, [=]() {
		Robot.Set(RightArm, OpenBrake);
	});
	//�ұ۹رձ�բ
	connect(ui.btn_right_break_close, &QPushButton::clicked, this, [=]() {
		Robot.Set(RightArm, CloseBrake);
	});


	//�����ǰ
	connect(ui.btn_left_arm_forword, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//������
	connect(ui.btn_left_arm_backward, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//�������
	connect(ui.btn_left_arm_left, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//�������
	connect(ui.btn_left_arm_right, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//�������
	connect(ui.btn_left_arm_up, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//�������
	connect(ui.btn_left_arm_down, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});


	//�ұ���ǰ
	connect(ui.btn_right_arm_forword, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//�ұ����
	connect(ui.btn_right_arm_backward, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//�ұ�����
	connect(ui.btn_right_arm_left, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//�ұ�����
	connect(ui.btn_right_arm_right, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//�ұ�����
	connect(ui.btn_right_arm_up, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//�ұ�����
	connect(ui.btn_right_arm_down, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});


	//��۽Ƕȼ�
	connect(ui.btn_L_joint_0_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_1_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_2_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_3_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[3] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_4_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[4] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_5_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[5] = -left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	//��۽Ƕ���
	connect(ui.btn_L_joint_0_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_1_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_2_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_3_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[3] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_4_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[4] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});
	connect(ui.btn_L_joint_5_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[5] = left_angle_scale;
		Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis_Left);
	});



	//�ұ۽Ƕȼ�
	connect(ui.btn_R_joint_0_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_1_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_2_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_3_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[3] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_4_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[4] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_5_dec, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[5] = -right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	//�ұ۽Ƕ���
	connect(ui.btn_R_joint_0_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_1_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_2_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_3_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[3] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_4_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[4] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});
	connect(ui.btn_R_joint_5_inc, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[5] = right_angle_scale;
		Robot.Set(RightArm, PTPRel_Angle, Angle_Dis_Right);
	});


	//ʾ�̲�������
	connect(ui.left_direction_slider, &QSlider::valueChanged, this, [&](int value) {
		left_direction_scale = float(value) / 100;
		ui.left_direction_value->setText(QString::number(left_direction_scale, 'f', 2));
	});
	connect(ui.left_angle_slider, &QSlider::valueChanged, this, [&](int value) {
		left_angle_scale = float(value) / 1000;
		ui.left_angle_value->setText(QString::number(left_angle_scale, 'f', 3));
	});
	connect(ui.right_direction_slider, &QSlider::valueChanged, this, [&](int value) {
		right_direction_scale = float(value) / 100;
		ui.right_direction_value->setText(QString::number(right_direction_scale, 'f', 2));
	});
	connect(ui.right_angle_slider, &QSlider::valueChanged, this, [&](int value) {
		right_angle_scale = float(value) / 1000;
		ui.right_angle_value->setText(QString::number(right_angle_scale, 'f', 3));
	});
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
