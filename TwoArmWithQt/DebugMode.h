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
	void on_btn_right_connect();
	void on_btn_left_break_open();
	void on_btn_left_break_close();
	void on_btn_right_break_open();
	void on_btn_right_break_close();


	/*********************退出*************************/
	void on_btn_exit();





};


