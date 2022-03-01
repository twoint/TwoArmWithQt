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
	int number;//选中的模式  0：调试  1：作业   2：服务器
	DebugMode* debugmode;//调试模式界面
	WorkMode* workmode;//作业模式界面
	ServeMode* servemode;//服务器模式界面

private slots:
	void on_btn_confirm();//确认按钮,进入相应模式
	void on_btn_exit();//退出按钮

};