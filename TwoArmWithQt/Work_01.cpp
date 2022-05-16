#pragma execution_character_set("utf-8")//���QLabel��������

#include "Work_01.h"
#include"RobotArm.h"

extern RobotArm Robot;



Work_01::Work_01(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//setWindowFlags(Qt::FramelessWindowHint | windowFlags());				//ȥ���ڱ߿�
	//setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��
	vrepTimer = new QTimer(this);											//vrepͼ��ˢ�¶�ʱ��
	cameraTimer = new QTimer(this);											//cameraͼ��ˢ�¶�ʱ��
	infoUpdateTimer = new QTimer(this);										//��������Ϣˢ�¶�ʱ��
	infoUpdateTimer->start(33);


	//��ʾ�����ʼ��ͼƬ
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));

	//��ʾcoppeliasim��ʼ��ͼƬ
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
	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
	//��ʾ�����ʼ��ͼƬ
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
	//������Ϣ
	if (Robot.LeftInf.Connect)
	{
		ui.label_left_connect->setText("������");
		ui.label_left_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_connect->setText("δ����");
		ui.label_left_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	if (Robot.RightInf.Connect)
	{
		ui.label_right_connect->setText("������");
		ui.label_right_connect->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_right_connect->setText("δ����");
		ui.label_right_connect->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");
	}

	//��բ��Ϣ
	if (Robot.LeftInf.Brake)
	{
		ui.label_left_break->setText("��բ�ѿ���");
		ui.label_left_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");
	}
	else
	{
		ui.label_left_break->setText("��բδ����");
		ui.label_left_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Brake)
	{
		ui.label_right_break->setText("��բ�ѿ���");
		ui.label_right_break->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_break->setText("��բδ����");
		ui.label_right_break->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	//�ű���Ϣ
	if (Robot.LeftInf.Script)
	{
		ui.label_left_script->setText("�ű���ȷ");
		ui.label_left_script->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_left_script->setText("�ű�����");
		ui.label_left_script->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Script)
	{
		ui.label_right_script->setText("�ű���ȷ");
		ui.label_right_script->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_script->setText("�ű�����");
		ui.label_right_script->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	//ͬ����Ϣ
	if (Robot.LeftInf.Sync)
	{
		ui.label_left_sync->setText("��ͬ��");
		ui.label_left_sync->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_left_sync->setText("δͬ��");
		ui.label_left_sync->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}

	if (Robot.RightInf.Sync)
	{
		ui.label_right_sync->setText("��ͬ��");
		ui.label_right_sync->setStyleSheet("QLabel{background-color:rgb(0,255,0);border-radius:10px;color:rgb(255,255,255);}");

	}
	else
	{
		ui.label_right_sync->setText("δͬ��");
		ui.label_right_sync->setStyleSheet("QLabel{background-color:rgb(255,0,0);border-radius:10px;color:rgb(255,255,255);}");

	}
}

/************************coppliasim����******************************/
void Work_01::importVrepFrame()
{
	cv::Mat channel(Robot.resolution[0], Robot.resolution[1], CV_8UC3, Robot.vrep_image);
	cv::Mat channelFlip;
	//��������ͼ�������Ǵ�ֱ��ת��,����Ӧ������cvMat �� v-rep ��ֱ������ķ����෴,flipһ�¾�������
	cv::flip(channel, channelFlip, 0);
	//��������ͼ������ʱrgbͨ���ֲ��ģ���cvMat Ĭ��bgr
	cv::cvtColor(channelFlip, channelFlip, cv::COLOR_RGB2BGR);
	QImage Qtemp = QImage((const unsigned char*)(channelFlip.data), channelFlip.cols, channelFlip.rows, channelFlip.step, QImage::Format_RGB888);
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(Qtemp));
	ui.label_vrep_show->setScaledContents(true);//ͼƬ������������Ļ
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
	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	vrepTimer->stop();

}

/************************�������******************************/
void Work_01::importCameraFrame()
{
	capture >> frame;
	cvtColor(frame, frame, CV_BGR2RGB);//only RGB of Qt
	QImage srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
	ui.label_camera_show->setPixmap(QPixmap::fromImage(srcQImage));
	ui.label_camera_show->setScaledContents(true);//ͼƬ������������Ļ
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
	//��ʾ�����ʼ��ͼƬ
	QImage image;
	image.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image));

	cameraTimer->stop();
	capture.release();
}


