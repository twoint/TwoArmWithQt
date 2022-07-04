#include "CameraShow.h"
#include<iostream>
using namespace std;

string video_adress = "rtsp://192.168.1.137:554/11/user=admin&password=&channel=1&stream=0.sdp?";

void CameraShow::OpenCamera()
{
	capture.open(video_adress);

	//if (!capture.open(0))
	//{
	//	cout << "相机无法打开" << endl;
	//}
	//else
	//{
	//	cout << "相机已打开" << endl;
	//}
}

void CameraShow::CloseCamera()
{
	capture.release();
}

void CameraShow::Show()
{
	capture.read(videoFrame);


	//if (!capture.read(videoFrame))
	//{
	//	cout << "无法获取帧" << endl;
	//}
	//else
	//{
	//	cout << "可以获取帧" << endl;
	//}
}
