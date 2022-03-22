#include "DebugMode.h"
#include"ModeSelect.h"
#include"ToolConnect.h"

#include"RobotArm.h"
#include"DebugPrintf.h"

extern ModeSelect* modeselect;
extern ToolConnect ToolConnectCom;
extern RobotArm Robot;

/*****��vrepͼ������ļ�******/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*******************************/


DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//ȥ���ڱ߿�
	setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	timer = new QTimer(this);											//vrepͼ��ˢ�¶�ʱ��


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
	//��������ͼ�������Ǵ�ֱ��ת��,����Ӧ������cvMat �� v-rep ��ֱ������ķ����෴,flipһ�¾�������
	cv::flip(channel, channelFlip, 0);
	//��������ͼ������ʱrgbͨ���ֲ��ģ���cvMat Ĭ��bgr
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


