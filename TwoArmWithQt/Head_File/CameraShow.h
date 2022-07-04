#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class CameraShow
{

public:
	bool showFlag = false;
	cv::VideoCapture capture;
	cv::Mat videoFrame;

	void OpenCamera();
	void Show();
	void CloseCamera();
};

