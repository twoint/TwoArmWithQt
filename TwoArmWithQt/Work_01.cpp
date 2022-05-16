#pragma execution_character_set("utf-8")//解决QLabel中文乱码

#include "Work_01.h"
#include"RobotArm.h"

extern RobotArm Robot;



Work_01::Work_01(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//去窗口边框
	//setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明
	vrepTimer = new QTimer(this);											//vrep图像刷新定时器
	cameraTimer = new QTimer(this);											//camera图像刷新定时器
	infoUpdateTimer = new QTimer(this);										//传感器信息刷新定时器
	infoUpdateTimer->start(33);


	//显示相机初始化图片
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));

	//显示coppeliasim初始化图片
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout

	connect(vrepTimer, SIGNAL(timeout()), this, SLOT(importVrepFrame()));//import frame when timeout
	connect(ui.btn_vrep_open, SIGNAL(clicked()), this, SLOT(on_btn_vrep_open()));
	connect(ui.btn_vrep_close, SIGNAL(clicked()), this, SLOT(on_btn_vrep_close()));

	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(importCameraFrame()));//import frame when timeout
	connect(ui.btn_camera_open, SIGNAL(clicked()), this, SLOT(on_btn_camera_open()));
	connect(ui.btn_camera_close, SIGNAL(clicked()), this, SLOT(on_btn_camera_close()));
}

Work_01::~Work_01()
{
}

void Work_01::on_btn_exit()
{
	//显示coppeliasim初始化图片
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
	//显示相机初始化图片
	QImage image;
	image.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image));

	vrepTimer->stop();
	cameraTimer->stop();
	capture.release();
	emit jumpPageTo(3);
}

void Work_01::infoUpdateFrame()
{
	//连接信息
	if (Robot.LeftInf.Connect)
	{
		ui.label_left_connect->setText("已连接");
		ui.label_left_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_connect->setText("未连接");
		ui.label_left_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	if (Robot.RightInf.Connect)
	{
		ui.label_right_connect->setText("已连接");
		ui.label_right_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_right_connect->setText("未连接");
		ui.label_right_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//抱闸信息
	if (Robot.LeftInf.Brake)
	{
		ui.label_left_break->setText("抱闸已开启");
		ui.label_left_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_break->setText("抱闸未开启");
		ui.label_left_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Brake)
	{
		ui.label_right_break->setText("抱闸已开启");
		ui.label_right_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_break->setText("抱闸未开启");
		ui.label_right_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	//脚本信息
	if (Robot.LeftInf.Script)
	{
		ui.label_left_script->setText("脚本正确");
		ui.label_left_script->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_left_script->setText("脚本错误");
		ui.label_left_script->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Script)
	{
		ui.label_right_script->setText("脚本正确");
		ui.label_right_script->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_script->setText("脚本错误");
		ui.label_right_script->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	//同步信息
	if (Robot.LeftInf.Sync)
	{
		ui.label_left_sync->setText("已同步");
		ui.label_left_sync->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_left_sync->setText("未同步");
		ui.label_left_sync->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Sync)
	{
		ui.label_right_sync->setText("已同步");
		ui.label_right_sync->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_sync->setText("未同步");
		ui.label_right_sync->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}
}

/************************coppliasim控制******************************/
void Work_01::importVrepFrame()
{
	cv::Mat channel(Robot.resolution[0], Robot.resolution[1], CV_8UC3, Robot.vrep_image);
	cv::Mat channelFlip;
	//读回来的图像数据是垂直翻转的,问题应该是在cvMat 和 v-rep 垂直坐标轴的方向相反,flip一下就正常了
	cv::flip(channel, channelFlip, 0);
	//读回来的图像数据时rgb通道分布的，而cvMat 默认bgr
	cv::cvtColor(channelFlip, channelFlip, cv::COLOR_RGB2BGR);
	QImage Qtemp = QImage((const unsigned char*)(channelFlip.data), channelFlip.cols, channelFlip.rows, channelFlip.step, QImage::Format_RGB888);
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(Qtemp));
	ui.label_vrep_show->setScaledContents(true);//图片缩放至整个屏幕
	//ui.label_vrep_show->resize(Qtemp.size());
	ui.label_vrep_show->show();
}

void Work_01::on_btn_vrep_open()
{
	vrepTimer->start(33);
	Robot.FlagVrepImg = true;
}

void Work_01::on_btn_vrep_close()
{
	//显示coppeliasim初始化图片
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	vrepTimer->stop();

}

/************************相机控制******************************/
void Work_01::importCameraFrame()
{
	capture >> frame;
	cvtColor(frame, frame, CV_BGR2RGB);//only RGB of Qt
	QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
	ui.label_camera_show->setPixmap(QPixmap::fromImage(srcQImage));
	ui.label_camera_show->setScaledContents(true);//图片缩放至整个屏幕
	//ui.label_camera_show->resize(srcQImage.size());
	ui.label_camera_show->show();
}

void Work_01::on_btn_camera_open()
{
	capture.open(0);
	cameraTimer->start(33);// Start timing, Signal out when timeout
}

void Work_01::on_btn_camera_close()
{
	//显示相机初始化图片
	QImage image;
	image.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image));

	cameraTimer->stop();
	capture.release();
}


