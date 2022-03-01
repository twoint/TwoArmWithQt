#pragma once
#include <qwidget.h>
#include"ui_ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"

#include<qbuttongroup.h>

class ModeSelect :
	public QWidget
{
	Q_OBJECT

public:
	ModeSelect(QWidget *parent = Q_NULLPTR);

private:
	Ui::ModeSelectClass ui;
	QButtonGroup* qButtonGroup;
	int number;//ѡ�е�ģʽ  0������  1����ҵ   2��������
	DebugMode* debugmode;//����ģʽ����
	WorkMode* workmode;//��ҵģʽ����
	ServeMode* servemode;//������ģʽ����

private slots:
	void on_btn_confirm();//ȷ�ϰ�ť,������Ӧģʽ
	void on_btn_exit();//�˳���ť

};