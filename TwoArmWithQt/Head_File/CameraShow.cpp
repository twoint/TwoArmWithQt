#include "CameraShow.h"
#include<iostream>
using namespace std;

string video_adress = "rtsp://192.168.1.137:554/11/user=admin&password=&channel=1&stream=0.sdp?";

void CameraShow::OpenCamera()
{
	capture.open(video_adress);

	//if (!capture.open(0))
	//{
	//	cout << "����޷���" << endl;
	//}
	//else
	//{
	//	cout << "����Ѵ�" << endl;
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
	//	cout << "�޷���ȡ֡" << endl;
	//}
	//else
	//{
	//	cout << "���Ի�ȡ֡" << endl;
	//}
}
