#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);
	void ThreadShow(const QString& str);//�����ã��������߳�����ui�ؼ�������ı�

private:
	Ui::DebugModeClass ui;


private slots:
	void on_btn_lead_gripper_on();//����ץ�ֿ�
	void on_btn_lead_gripper_off();//����ץ�ֹ�
	void on_btn_arrester_grab_on();//������ץ�ֿ�
	void on_btn_arrester_grab_off();//������ץ�ֹ�


	
	void on_btn_exit();
};
