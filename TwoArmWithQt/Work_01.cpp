#pragma execution_character_set("utf-8")//���QLabel��������

#include "Work_01.h"
#include"RobotArm.h"

extern RobotArm Robot;

string video_adress = "rtsp://192.168.1.137:554/11/user=admin&password=&channel=1&stream=0.sdp?";

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

	ui.tb_init->setCheckable(true);
	ui.tb_cv->setCheckable(true);
	ui.tb_grab->setCheckable(true);
	ui.tb_punch->setCheckable(true);
	ui.tb_fire->setCheckable(true);
	ui.stackedWidget->setCurrentIndex(0);


	//��ʾ�����ʼ��ͼƬ
	QImage image_camera_init;
	image_camera_init.load(".\\picture\\debug_mode\\camera.png");
	ui.label_camera_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_camera_show->setPixmap(QPixmap::fromImage(image_camera_init));
	//ui.label_camera_show->setScaledContents(true);//ͼƬ������������Ļ

	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
	//ui.label_vrep_show->setScaledContents(true);//ͼƬ������������Ļ

	signalSlotConnect();
}

Work_01::~Work_01()
{
}

void Work_01::signalSlotConnect()
{
	//�˳���ť
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	//��������Ϣ����
	connect(infoUpdateTimer, SIGNAL(timeout()), this, SLOT(infoUpdateFrame()));//import frame when timeout
	//vrep
	connect(vrepTimer, SIGNAL(timeout()), this, SLOT(importVrepFrame()));//import frame when timeout
	connect(ui.btn_vrep_show_switch, SIGNAL(clicked()), this, SLOT(on_btn_vrep_show_switch()));
	//���
	connect(cameraTimer, SIGNAL(timeout()), this, SLOT(importCameraFrame()));//import frame when timeout
	connect(ui.btn_camera_show_switch, SIGNAL(clicked()), this, SLOT(on_btn_camera_show_switch()));
	//��ҵ����
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
	//����������ݻ��ߴ�������
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	//��ȡͼƬ�ߴ�
	int imageWidth = size.width();
	int imageHeight = size.height();

	//�����ߴ��ͼƬ,��֤ͼƬ��ʾ��������
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// ��ͼƬ�ü�ΪԲ��
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void Work_01::on_btn_exit()
{
	//��ʾcoppeliasim��ʼ��ͼƬ
	QImage image_coppeliasim_init;
	image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setScaledContents(false);
	ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));

	//��ʾ�����ʼ��ͼƬ
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
	ui.label_vrep_show->setPixmap(getRoundRectPixmap(QPixmap::fromImage(Qtemp), Qtemp.size(), 20));//��ʾԲ��ͼƬ
	//ui.label_vrep_show->setPixmap(QPixmap::fromImage(Qtemp));
	ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
	ui.label_vrep_show->setScaledContents(true);//ͼƬ������������Ļ
	//ui.label_vrep_show->resize(Qtemp.size());
}

void Work_01::on_btn_vrep_show_switch()
{
	vrep_show_flag = !vrep_show_flag;
	if (vrep_show_flag)//��
	{
		vrepTimer->start(33);
		Robot.FlagVrepImg = true;
		ui.plainTextEdit->appendPlainText("vrep�Ѵ�...");

	}
	else//�ر�
	{
		//��ʾcoppeliasim��ʼ��ͼƬ
		QImage image_coppeliasim_init;
		image_coppeliasim_init.load(".\\picture\\debug_mode\\coppeliasim.png");
		ui.label_vrep_show->setPixmap(QPixmap::fromImage(image_coppeliasim_init));
		ui.label_vrep_show->setScaledContents(false);//ͼƬ������������Ļ�ر�
		ui.label_vrep_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");
		vrepTimer->stop();
		ui.plainTextEdit->appendPlainText("vrep�ѹر�...");
	}
}

/************************�������******************************/
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
		ui.label_camera_show->setPixmap(getRoundRectPixmap(QPixmap::fromImage(srcQImage), srcQImage.size(), 20));//��ʾԲ��ͼƬ
		//ui.label_camera_show->setPixmap(QPixmap::fromImage(srcQImage));
		ui.label_camera_show->setStyleSheet("border-radius:20px;");
		ui.label_camera_show->setScaledContents(true);//ͼƬ������������Ļ
		//ui.label_camera_show->resize(srcQImage.size());
		ui.plainTextEdit->appendPlainText("show frame");
	}
	
}

void Work_01::on_btn_camera_show_switch()
{
	camera_show_flag = !camera_show_flag;
	if (camera_show_flag)//��
	{
		//capture.open(0);
		//capture.open(video_adress);
		if (!capture.open(video_adress))
		{
			ui.plainTextEdit->appendPlainText("����޷���...");
		}
		else
		{
			cameraTimer->start(20);// Start timing, Signal out when timeout
			ui.plainTextEdit->appendPlainText("����Ѵ�...");
		}
	}
	else//�ر�
	{
		//��ʾ�����ʼ��ͼƬ
		QImage image;
		image.load(".\\picture\\debug_mode\\camera.png");
		ui.label_camera_show->setPixmap(QPixmap::fromImage(image));
		ui.label_camera_show->setStyleSheet("border-radius:20px;background-color: rgba(255, 255, 255, 0);");

		cameraTimer->stop();
		capture.release();
		ui.plainTextEdit->appendPlainText("����ѹر�...");
	}
}

/*************************��ҵ����*****************************/

void Work_01::on_tb_init()
{
	ui.plainTextEdit->appendPlainText("�����ʼ����ҵ");
	selectToolButton(ui.tb_init);
}

void Work_01::on_tb_cv()
{
	ui.plainTextEdit->appendPlainText("����ʶ����ҵ");
	selectToolButton(ui.tb_cv);
}

void Work_01::on_tb_grab()
{
	ui.plainTextEdit->appendPlainText("����ץ����ҵ");
	selectToolButton(ui.tb_grab);
}

void Work_01::on_tb_punch()
{
	ui.plainTextEdit->appendPlainText("���봩����ҵ");
	selectToolButton(ui.tb_punch);
}

void Work_01::on_tb_fire()
{
	ui.plainTextEdit->appendPlainText("����ӻ���ҵ");
	selectToolButton(ui.tb_fire);
}




