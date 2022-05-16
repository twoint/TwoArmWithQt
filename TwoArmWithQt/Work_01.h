#pragma once

#include <QWidget>
#include "ui_Work_01.h"
#include <QTimer>


/*****��vrepͼ������ļ�******/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*******************************/


class Work_01 : public QWidget
{
	Q_OBJECT

public:
	Work_01(QWidget *parent = Q_NULLPTR);
	~Work_01();

private:
	Ui::Work_01 ui;
	QTimer *vrepTimer;//vrepͼ��ˢ��
	QTimer *cameraTimer;//cameraͼ��ˢ��
	QTimer *infoUpdateTimer;//��������Ϣˢ��
	cv::VideoCapture capture;
	cv::Mat frame;

signals:
	void jumpPageTo(int n);

private slots:
	void importVrepFrame();//vrepͼ��ˢ��
	void on_btn_vrep_open();//��vrepͼ��
	void on_btn_vrep_close();//�ر�vrepͼ��

	void importCameraFrame();//���ͼ��ˢ��
	void on_btn_camera_open();//�����ͼ��
	void on_btn_camera_close();//�ر����ͼ��

	void infoUpdateFrame();

	void on_btn_exit();

};
