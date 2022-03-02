#include "DebugMode.h"
#include"ModeSelect.h"
#include"ToolConnect.h"

extern ModeSelect* modeselect;
extern ToolConnect ToolConnectCom;


DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//去窗口边框
	setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(ui.btn_lead_gripper_on, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_on()));
	connect(ui.btn_lead_gripper_off, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_off()));
	connect(ui.btn_arrester_grab_on, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_on()));
	connect(ui.btn_arrester_grab_off, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_off()));
}

void DebugMode::on_btn_lead_gripper_on()
{
	ToolConnectCom.command(1, 1, 0, 0);
}

void DebugMode::on_btn_lead_gripper_off()
{
	ToolConnectCom.command(1, 2, 0, 0);
}

void DebugMode::on_btn_arrester_grab_on()
{
	ToolConnectCom.command(2, 1, 0, 0);
}

void DebugMode::on_btn_arrester_grab_off()
{
	ToolConnectCom.command(2, 2, 0, 0);
}

void DebugMode::on_btn_exit()
{
	modeselect->show();
	this->close();
}

void DebugMode::ThreadShow(const QString & str)
{
	ui.plainTextEdit->insertPlainText(str);
}


