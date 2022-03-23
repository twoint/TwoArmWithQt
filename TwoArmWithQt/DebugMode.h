#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"
#include <QTimer>

/*****打开vrep图像相关文件******/
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
	void ThreadShow(const QString& str);//调试用，在其它线程中向ui控件中添加文本

private:
	Ui::DebugModeClass ui;
	QTimer *vrepTimer;//vrep图像刷新
	QTimer *cameraTimer;//camera图像刷新
	cv::VideoCapture capture;
	cv::Mat frame;




private slots:
	void on_btn_lead_gripper_on();//引线抓手开
	void on_btn_lead_gripper_off();//引线抓手关
	void on_btn_arrester_grab_on();//避雷器抓手开
	void on_btn_arrester_grab_off();//避雷器抓手关

	void importVrepFrame();//vrep图像刷新
	void on_btn_vrep_open();//打开vrep图像
	void on_btn_vrep_close();//关闭vrep图像

	void importCameraFrame();//相机图像刷新
	void on_btn_camera_open();//打开相机图像
	void on_btn_camera_close();//关闭相机图像

	void on_btn_exit();
};
