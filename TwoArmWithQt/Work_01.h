#pragma once

#include <QWidget>
#include "ui_Work_01.h"
#include <QTimer>
#include<qpainter.h>


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

	void signalSlotConnect();
	void selectToolButton(QToolButton *pTlb);
	QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius);//处理圆角图片

	bool vrep_show_flag = false;
	bool camera_show_flag = false;


signals:
	void jumpPageTo(int n);

private slots:
	void importVrepFrame();//vrep图像刷新
	void on_btn_vrep_show_switch();

	void importCameraFrame();//相机图像刷新
	void on_btn_camera_show_switch();

	//作业步骤
	void on_tb_init();
	void on_tb_cv();
	void on_tb_grab();
	void on_tb_punch();
	void on_tb_fire();

	void infoUpdateFrame();
	void on_btn_exit();


	//测试
	//左臂映射
	void on_btn_4();

};
