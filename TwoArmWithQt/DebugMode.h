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
	QTimer *infoUpdateTimer;//��������Ϣˢ��

signals:
	void jumpPageTo(int n);

private slots:

	void infoUpdateFrame();

	/*********************���߿���*************************/
	//void on_btn_lead_gripper_on();//����ץ�ֿ�
	//void on_btn_lead_gripper_off();//����ץ�ֹ�
	//void on_btn_arrester_grab_on();//������ץ�ֿ�
	//void on_btn_arrester_grab_off();//������ץ�ֹ�

	/*********************�������*************************/
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

	/*********************��е���������ӡ���բ����*************************/
	void on_btn_left_connect();
	void on_btn_right_connect();
	void on_btn_left_break_open();
	void on_btn_left_break_close();
	void on_btn_right_break_open();
	void on_btn_right_break_close();


	/*********************�˳�*************************/
	void on_btn_exit();





};


