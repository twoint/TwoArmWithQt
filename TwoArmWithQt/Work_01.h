#pragma once

#include <QWidget>
#include "ui_Work_01.h"
#include <QTimer>


/*****打开vrep图像相关文件******/
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
	QTimer *vrepTimer;//vrep图像刷新
	QTimer *cameraTimer;//camera图像刷新
	QTimer *infoUpdateTimer;//传感器信息刷新
	cv::VideoCapture capture;
	cv::Mat frame;

signals:
	void jumpPageTo(int n);

private slots:
	void importVrepFrame();//vrep图像刷新
	void on_btn_vrep_open();//打开vrep图像
	void on_btn_vrep_close();//关闭vrep图像

	void importCameraFrame();//相机图像刷新
	void on_btn_camera_open();//打开相机图像
	void on_btn_camera_close();//关闭相机图像

	void infoUpdateFrame();

	void on_btn_exit();

};
