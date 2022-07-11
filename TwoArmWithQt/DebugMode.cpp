#pragma execution_character_set("utf-8")//解决QLabel中文乱码

#include "DebugMode.h"
#include"ModeSelect.h"
#include"DebugPrintf.h"
#include"RobotArm.h"
#include"ToolConnect.h"

extern RobotArm Robot;
extern ToolConnect ToolConnectCom;

static float left_direction_scale = 0.5f;	//左臂方向示教比例
static float right_direction_scale = 0.5f;	//右臂方向示教比例                                
static float left_angle_scale = 0.02f;		//左臂角度示教比例
static float right_angle_scale = 0.02f;		//右臂角度示教比例

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//去窗口边框
	//setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明
	infoUpdateTimer = new QTimer(this);										//传感器信息刷新定时器
	infoUpdateTimer->start(33);


	ui.left_direction_slider->setValue(int(left_direction_scale * 100));
	ui.left_direction_value->setText(QString::number(left_direction_scale, 'f', 2));
	ui.left_angle_slider->setValue(int(left_angle_scale * 1000));
	ui.left_angle_value->setText(QString::number(left_angle_scale, 'f', 3));
	ui.right_direction_slider->setValue(int(right_direction_scale * 100));
	ui.right_direction_value->setText(QString::number(right_direction_scale, 'f', 2));
	ui.right_angle_slider->setValue(int(right_angle_scale * 1000));
	ui.right_angle_value->setText(QString::number(right_angle_scale, 'f', 3));


	//信息刷新
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout
	//退出
	connect(ui.btn_exit, &QPushButton::clicked, this, [=]() {
		emit jumpPageTo(1);
	});
	//左臂连接
	connect(ui.btn_left_connect, &QPushButton::clicked, this, [=]() {
		Robot.CreatSocketLeft("192.168.1.100", 8010);
	});
	//左臂断开连接
	connect(ui.btn_left_disconnect, &QPushButton::clicked, this, [=]() {
		Robot.CloseSocketLeft();
	});
	//右臂连接
	connect(ui.btn_right_connect, &QPushButton::clicked, this, [=]() {
		Robot.CreatSocketRight("192.168.1.101", 8010);
	});
	//右臂断开连接
	connect(ui.btn_right_disconnect, &QPushButton::clicked, this, [=]() {
		Robot.CloseSocketRight();
	});
	//左臂打开抱闸
	connect(ui.btn_left_break_open, &QPushButton::clicked, this, [=]() {
		Robot.Set(LeftArm, OpenBrake);
	});
	//左臂关闭抱闸
	connect(ui.btn_left_break_close, &QPushButton::clicked, this, [=]() {
		Robot.Set(LeftArm, CloseBrake);
	});
	//右臂打开抱闸
	connect(ui.btn_right_break_open, &QPushButton::clicked, this, [=]() {
		Robot.Set(RightArm, OpenBrake);
	});
	//右臂关闭抱闸
	connect(ui.btn_right_break_close, &QPushButton::clicked, this, [=]() {
		Robot.Set(RightArm, CloseBrake);
	});


	//左臂向前
	connect(ui.btn_left_arm_forword, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//左臂向后
	connect(ui.btn_left_arm_backward, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[1] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//左臂向左
	connect(ui.btn_left_arm_left, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//左臂向右
	connect(ui.btn_left_arm_right, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[0] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//左臂向上
	connect(ui.btn_left_arm_up, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});
	//左臂向下
	connect(ui.btn_left_arm_down, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Left[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Left[2] = -left_direction_scale;
		Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis_Left);
	});


	//右臂向前
	connect(ui.btn_right_arm_forword, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//右臂向后
	connect(ui.btn_right_arm_backward, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[1] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//右臂向左
	connect(ui.btn_right_arm_left, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//右臂向右
	connect(ui.btn_right_arm_right, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[0] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//右臂向上
	connect(ui.btn_right_arm_up, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});
	//右臂向下
	connect(ui.btn_right_arm_down, &QPushButton::clicked, this, [=]() {
		float Angle_Dis_Right[6] = { 0,0,0,0,0,0 };
		Angle_Dis_Right[2] = -right_direction_scale;
		Robot.Set(RightArm, PTPRel_TCP, Angle_Dis_Right);
	});


	//左臂角度减
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
	//左臂角度增
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



	//右臂角度减
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
	//右臂角度增
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


	//示教参数设置
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
	//左臂连接信息
	if (!Robot.LeftInf.Connect)
	{
		ui.label_left_arm_connect->setText("未连接");
		ui.label_left_arm_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_arm_connect->setText("已连接");
		ui.label_left_arm_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//右臂连接信息
	if (!Robot.RightInf.Connect)
	{
		ui.label_right_arm_connect->setText("未连接");
		ui.label_right_arm_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_right_arm_connect->setText("已连接");
		ui.label_right_arm_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//左臂抱闸信息
	if (!Robot.LeftInf.Brake)
	{
		ui.label_left_arm_break->setText("抱闸关闭");
		ui.label_left_arm_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_arm_break->setText("抱闸开启");
		ui.label_left_arm_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	//右臂抱闸信息
	if (!Robot.RightInf.Brake)
	{
		ui.label_right_arm_break->setText("抱闸关闭");
		ui.label_right_arm_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_arm_break->setText("抱闸开启");
		ui.label_right_arm_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	//左臂角度信息
	ui.label_L_joint_0->setText(QString::number(Robot.LeftInf.Angle[0], 'f', 2));
	ui.label_L_joint_1->setText(QString::number(Robot.LeftInf.Angle[1], 'f', 2));
	ui.label_L_joint_2->setText(QString::number(Robot.LeftInf.Angle[2], 'f', 2));
	ui.label_L_joint_3->setText(QString::number(Robot.LeftInf.Angle[3], 'f', 2));
	ui.label_L_joint_4->setText(QString::number(Robot.LeftInf.Angle[4], 'f', 2));
	ui.label_L_joint_5->setText(QString::number(Robot.LeftInf.Angle[5], 'f', 2));
	//右臂角度信息
	ui.label_R_joint_0->setText(QString::number(Robot.RightInf.Angle[0], 'f', 2));
	ui.label_R_joint_1->setText(QString::number(Robot.RightInf.Angle[1], 'f', 2));
	ui.label_R_joint_2->setText(QString::number(Robot.RightInf.Angle[2], 'f', 2));
	ui.label_R_joint_3->setText(QString::number(Robot.RightInf.Angle[3], 'f', 2));
	ui.label_R_joint_4->setText(QString::number(Robot.RightInf.Angle[4], 'f', 2));
	ui.label_R_joint_5->setText(QString::number(Robot.RightInf.Angle[5], 'f', 2));
}
