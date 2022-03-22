#include "DebugMode.h"
#include"ModeSelect.h"
#include"ToolConnect.h"

#include"RobotArm.h"
#include"DebugPrintf.h"

extern ModeSelect* modeselect;
extern ToolConnect ToolConnectCom;
extern RobotArm Robot;

/*****打开vrep图像相关文件******/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*******************************/


DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//去窗口边框
	setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明
	timer = new QTimer(this);											//vrep图像刷新定时器


	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(ui.btn_lead_gripper_on, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_on()));
	connect(ui.btn_lead_gripper_off, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_off()));
	connect(ui.btn_arrester_grab_on, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_on()));
	connect(ui.btn_arrester_grab_off, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_off()));

	connect(timer, SIGNAL(timeout()), this, SLOT(importFrame()));//import frame when timeout
	connect(ui.btn_vrep_open, SIGNAL(clicked()), this, SLOT(on_btn_vrep_open()));
	connect(ui.btn_vrep_close, SIGNAL(clicked()), this, SLOT(on_btn_vrep_close()));
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

void DebugMode::importFrame()
{
	cv::Mat channel(Robot.resolution[0], Robot.resolution[1], CV_8UC3, Robot.vrep_image);
	cv::Mat channelFlip;
	//读回来的图像数据是垂直翻转的,问题应该是在cvMat 和 v-rep 垂直坐标轴的方向相反,flip一下就正常了
	cv::flip(channel, channelFlip, 0);
	//读回来的图像数据时rgb通道分布的，而cvMat 默认bgr
	cv::cvtColor(channelFlip, channelFlip, cv::COLOR_RGB2BGR);
	QImage Qtemp = QImage((const unsigned char*)(channelFlip.data), channelFlip.cols, channelFlip.rows, channelFlip.step, QImage::Format_RGB888);
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(Qtemp));
	ui.label_vrep_show->resize(Qtemp.size());
	ui.label_vrep_show->show();


}

void DebugMode::on_btn_vrep_open()
{
	timer->start(33);
	Robot.FlagVrepImg = true;
}

void DebugMode::on_btn_vrep_close()
{
	timer->stop();

}


