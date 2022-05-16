#include "DebugMode.h"
#include"ModeSelect.h"
#include"ToolConnect.h"
#include"RobotArm.h"
#include"DebugPrintf.h"

extern ModeSelect* modeselect;
extern ToolConnect ToolConnectCom;
extern RobotArm Robot;



DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//ȥ���ڱ߿�
	//setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	vrepTimer = new QTimer(this);											//vrepͼ��ˢ�¶�ʱ��
	cameraTimer = new QTimer(this);											//cameraͼ��ˢ�¶�ʱ��

	//��ʾ�����ʼ��ͼƬ
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));
	
	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(ui.btn_lead_gripper_on, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_on()));
	connect(ui.btn_lead_gripper_off, SIGNAL(clicked()), this, SLOT(on_btn_lead_gripper_off()));
	connect(ui.btn_arrester_grab_on, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_on()));
	connect(ui.btn_arrester_grab_off, SIGNAL(clicked()), this, SLOT(on_btn_arrester_grab_off()));

	connect(vrepTimer, SIGNAL(timeout()), this, SLOT(importVrepFrame()));//import frame when timeout
	connect(ui.btn_vrep_open, SIGNAL(clicked()), this, SLOT(on_btn_vrep_open()));
	connect(ui.btn_vrep_close, SIGNAL(clicked()), this, SLOT(on_btn_vrep_close()));

	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(importCameraFrame()));//import frame when timeout
	connect(ui.btn_camera_open, SIGNAL(clicked()), this, SLOT(on_btn_camera_open()));
	connect(ui.btn_camera_close, SIGNAL(clicked()), this, SLOT(on_btn_camera_close()));
}


void DebugMode::on_btn_exit()
{
	vrepTimer->stop();
	cameraTimer->stop();
	capture.release();
	emit jumpPageTo(1);
}

void DebugMode::ThreadShow(const QString & str)
{
	ui.plainTextEdit->insertPlainText(str);
}

/************************���߿���******************************/
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



/************************coppliasim����******************************/
void DebugMode::importVrepFrame()
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
	vrepTimer->start(33);
	Robot.FlagVrepImg = true;
}

void DebugMode::on_btn_vrep_close()
{
	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	vrepTimer->stop();

}

/************************�������******************************/
void DebugMode::importCameraFrame()
{
	capture >> frame;
	cvtColor(frame, frame, CV_BGR2RGB);//only RGB of Qt
	QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
	ui.label_camera_show->setPixmap(QPixmap::fromImage(srcQImage));
	ui.label_camera_show->resize(srcQImage.size());
	ui.label_camera_show->show();
}

void DebugMode::on_btn_camera_open()
{
	capture.open(0);
	cameraTimer->start(33);// Start timing, Signal out when timeout
}

void DebugMode::on_btn_camera_close()
{
	//��ʾ�����ʼ��ͼƬ
	QImage image;
	image.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image));

	cameraTimer->stop();
	capture.release();
}


