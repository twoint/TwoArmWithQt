#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"
#include <QTimer>

/*****��vrepͼ������ļ�******/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*******************************/


class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);
	void ThreadShow(const QString& str);//�����ã��������߳�����ui�ؼ�������ı�

private:
	Ui::DebugModeClass ui;
	QTimer *vrepTimer;//vrepͼ��ˢ��
	QTimer *cameraTimer;//cameraͼ��ˢ��
	cv::VideoCapture capture;
	cv::Mat frame;




private slots:
	void on_btn_lead_gripper_on();//����ץ�ֿ�
	void on_btn_lead_gripper_off();//����ץ�ֹ�
	void on_btn_arrester_grab_on();//������ץ�ֿ�
	void on_btn_arrester_grab_off();//������ץ�ֹ�

	void importVrepFrame();//vrepͼ��ˢ��
	void on_btn_vrep_open();//��vrepͼ��
	void on_btn_vrep_close();//�ر�vrepͼ��

	void importCameraFrame();//���ͼ��ˢ��
	void on_btn_camera_open();//�����ͼ��
	void on_btn_camera_close();//�ر����ͼ��

	void on_btn_exit();
};
