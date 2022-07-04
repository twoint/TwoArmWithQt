#pragma execution_character_set("utf-8")//解决QLabel中文乱码

#include "Work_01.h"
#include"RobotArm.h"

extern RobotArm Robot;

string video_adress = "rtsp://192.168.1.137:554/11/user=admin&password=&channel=1&stream=0.sdp?";

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

	ui.tb_init->setCheckable(true);
	ui.tb_cv->setCheckable(true);
	ui.tb_grab->setCheckable(true);
	ui.tb_punch->setCheckable(true);
	ui.tb_fire->setCheckable(true);
	ui.stackedWidget->setCurrentIndex(0);


	//显示相机初始化图片
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));
	//ui.label_camera_show->setScaledContents(true);//图片缩放至整个屏幕

	//显示coppeliasim初始化图片
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
	//ui.label_vrep_show->setScaledContents(true);//图片缩放至整个屏幕

	signalSlotConnect();
}

Work_01::~Work_01()
{
}

void Work_01::signalSlotConnect()
{
	//退出按钮
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	//传感器信息更新
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout
	//vrep
	connect(vrepTimer, SIGNAL(timeout()), this, SLOT(importVrepFrame()));//import frame when timeout
	connect(ui.btn_vrep_show_switch, SIGNAL(clicked()), this, SLOT(on_btn_vrep_show_switch()));
	//相机
	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(importCameraFrame()));//import frame when timeout
	connect(ui.btn_camera_show_switch, SIGNAL(clicked()), this, SLOT(on_btn_camera_show_switch()));
	//作业步骤
	connect(ui.tb_init, SIGNAL(clicked()), this, SLOT(on_tb_init()));
	connect(ui.tb_cv, SIGNAL(clicked()), this, SLOT(on_tb_cv()));
	connect(ui.tb_grab, SIGNAL(clicked()), this, SLOT(on_tb_grab()));
	connect(ui.tb_punch, SIGNAL(clicked()), this, SLOT(on_tb_punch()));
	connect(ui.tb_fire, SIGNAL(clicked()), this, SLOT(on_tb_fire()));

	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(on_btn_4()));
}

void Work_01::selectToolButton(QToolButton * pTlb)
{
	if (ui.tb_init == pTlb)
	{
		ui.stackedWidget->setCurrentIndex(0);
		ui.tb_init->setChecked(true);
		ui.tb_cv->setChecked(false);
		ui.tb_grab->setChecked(false);
		ui.tb_punch->setChecked(false);
		ui.tb_fire->setChecked(false);
	}
	else if (ui.tb_cv == pTlb)
	{
		ui.stackedWidget->setCurrentIndex(1);
		ui.tb_init->setChecked(false);
		ui.tb_cv->setChecked(true);
		ui.tb_grab->setChecked(false);
		ui.tb_punch->setChecked(false);
		ui.tb_fire->setChecked(false);
	}
	else if (ui.tb_grab == pTlb)
	{
		ui.stackedWidget->setCurrentIndex(2);
		ui.tb_init->setChecked(false);
		ui.tb_cv->setChecked(false);
		ui.tb_grab->setChecked(true);
		ui.tb_punch->setChecked(false);
		ui.tb_fire->setChecked(false);
	}
	else if (ui.tb_punch == pTlb)
	{
		ui.stackedWidget->setCurrentIndex(3);
		ui.tb_init->setChecked(false);
		ui.tb_cv->setChecked(false);
		ui.tb_grab->setChecked(false);
		ui.tb_punch->setChecked(true);
		ui.tb_fire->setChecked(false);
	}
	else if (ui.tb_fire == pTlb)
	{
		ui.stackedWidget->setCurrentIndex(4);
		ui.tb_init->setChecked(false);
		ui.tb_cv->setChecked(false);
		ui.tb_grab->setChecked(false);
		ui.tb_punch->setChecked(false);
		ui.tb_fire->setChecked(true);
	}

}

QPixmap Work_01::getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius)
{
	//不处理空数据或者错误数据
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	//获取图片尺寸
	int imageWidth = size.width();
	int imageHeight = size.height();

	//处理大尺寸的图片,保证图片显示区域完整
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void Work_01::on_btn_exit()
{
	//显示coppeliasim初始化图片
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setScaledContents(false);
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	//显示相机初始化图片
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_camera_show->setScaledContents(false);
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));

	vrepTimer->stop();
	cameraTimer->stop();
	capture.release();
	emit jumpPageTo(3);
}

void Work_01::on_btn_4()
{
	Robot.LeftInf.Sync = !Robot.LeftInf.Sync;
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
	ui.label_vrep_show->setPixmap(getRoundRectPixmap(QPixmap::fromImage(Qtemp), Qtemp.size(), 20));//显示圆角图片
	//ui.label_vrep_show->setPixmap(QPixmap::fromImage(Qtemp));
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setScaledContents(true);//图片缩放至整个屏幕
	//ui.label_vrep_show->resize(Qtemp.size());
}

void Work_01::on_btn_vrep_show_switch()
{
	vrep_show_flag = !vrep_show_flag;
	if (vrep_show_flag)//打开
	{
		vrepTimer->start(33);
		Robot.FlagVrepImg = true;
		ui.plainTextEdit->appendPlainText("vrep已打开...");

	}
	else//关闭
	{
		//显示coppeliasim初始化图片
		QImage image_coppeliasim_init;
		image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
		ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
		ui.label_vrep_show->setScaledContents(false);//图片缩放至整个屏幕关闭
		ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
		vrepTimer->stop();
		ui.plainTextEdit->appendPlainText("vrep已关闭...");
	}
}

/************************相机控制******************************/
void Work_01::importCameraFrame()
{
	//capture >> videoFrame;
	if (!capture.read(videoFrame))
	{
		ui.plainTextEdit->appendPlainText("no frame");
		//capture.release();
		capture.open(video_adress);
	}
	else
	{
		cvtColor(videoFrame, videoFrame, CV_BGR2RGB);//only RGB of Qt
		srcQImage = QImage((uchar*)(videoFrame.data), videoFrame.cols, videoFrame.rows, QImage::Format_RGB888);
		ui.label_camera_show->setPixmap(getRoundRectPixmap(QPixmap::fromImage(srcQImage), srcQImage.size(), 20));//显示圆角图片
		//ui.label_camera_show->setPixmap(QPixmap::fromImage(srcQImage));
		ui.label_camera_show->setStyleSheet("border-radius:20px;");
		ui.label_camera_show->setScaledContents(true);//图片缩放至整个屏幕
		//ui.label_camera_show->resize(srcQImage.size());
		ui.plainTextEdit->appendPlainText("show frame");
	}
	
}

void Work_01::on_btn_camera_show_switch()
{
	camera_show_flag = !camera_show_flag;
	if (camera_show_flag)//打开
	{
		//capture.open(0);
		//capture.open(video_adress);
		if (!capture.open(video_adress))
		{
			ui.plainTextEdit->appendPlainText("相机无法打开...");
		}
		else
		{
			cameraTimer->start(20);// Start timing, Signal out when timeout
			ui.plainTextEdit->appendPlainText("相机已打开...");
		}
	}
	else//关闭
	{
		//显示相机初始化图片
		QImage image;
		image.load(".\\picture\\debug_mode\\camera.png");
		ui.label_camera_show->setPixmap(QPixmap::fromImage(image));
		ui.label_camera_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");

		cameraTimer->stop();
		capture.release();
		ui.plainTextEdit->appendPlainText("相机已关闭...");
	}
}

/*************************作业步骤*****************************/

void Work_01::on_tb_init()
{
	ui.plainTextEdit->appendPlainText("进入初始化作业");
	selectToolButton(ui.tb_init);
}

void Work_01::on_tb_cv()
{
	ui.plainTextEdit->appendPlainText("进入识别作业");
	selectToolButton(ui.tb_cv);
}

void Work_01::on_tb_grab()
{
	ui.plainTextEdit->appendPlainText("进入抓线作业");
	selectToolButton(ui.tb_grab);
}

void Work_01::on_tb_punch()
{
	ui.plainTextEdit->appendPlainText("进入穿线作业");
	selectToolButton(ui.tb_punch);
}

void Work_01::on_tb_fire()
{
	ui.plainTextEdit->appendPlainText("进入接火作业");
	selectToolButton(ui.tb_fire);
}




