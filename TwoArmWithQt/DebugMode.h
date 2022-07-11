#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"
#include <QTimer>

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);


private:
	Ui::DebugModeClass ui;
	QTimer *infoUpdateTimer;//传感器信息刷新

signals:
	void jumpPageTo(int n);

private slots:

	void infoUpdateFrame();

	/*********************工具控制*************************/
	//void on_btn_lead_gripper_on();//引线抓手开
	//void on_btn_lead_gripper_off();//引线抓手关
	//void on_btn_arrester_grab_on();//避雷器抓手开
	//void on_btn_arrester_grab_off();//避雷器抓手关

	/*********************方向控制*************************/
	void on_btn_left_arm_forward();
	void on_btn_left_arm_backward();
	void on_btn_left_arm_left();
	void on_btn_left_arm_right();
	void on_btn_left_arm_up();
	void on_btn_left_arm_down();

	void on_btn_right_arm_forward();
	void on_btn_right_arm_backward();
	void on_btn_right_arm_left();
	void on_btn_right_arm_right();
	void on_btn_right_arm_up();
	void on_btn_right_arm_down();

	/*********************机械臂网络连接、抱闸设置*************************/
	void on_btn_left_connect();
	void on_btn_left_disconnect();
	void on_btn_right_connect();
	void on_btn_right_disconnect();
	void on_btn_left_break_open();
	void on_btn_left_break_close();
	void on_btn_right_break_open();
	void on_btn_right_break_close();

	/*********************机械臂角度值修改*************************/
	void on_btn_L_joint_0_dec();
	void on_btn_L_joint_1_dec();
	void on_btn_L_joint_2_dec();
	void on_btn_L_joint_3_dec();
	void on_btn_L_joint_4_dec();
	void on_btn_L_joint_5_dec();

	void on_btn_L_joint_0_inc();
	void on_btn_L_joint_1_inc();
	void on_btn_L_joint_2_inc();
	void on_btn_L_joint_3_inc();
	void on_btn_L_joint_4_inc();
	void on_btn_L_joint_5_inc();

	void on_btn_R_joint_0_dec();
	void on_btn_R_joint_1_dec();
	void on_btn_R_joint_2_dec();
	void on_btn_R_joint_3_dec();
	void on_btn_R_joint_4_dec();
	void on_btn_R_joint_5_dec();

	void on_btn_R_joint_0_inc();
	void on_btn_R_joint_1_inc();
	void on_btn_R_joint_2_inc();
	void on_btn_R_joint_3_inc();
	void on_btn_R_joint_4_inc();
	void on_btn_R_joint_5_inc();

	//停止
	void on_btn_stop();

	/*********************退出*************************/
	//void on_btn_exit();





};


