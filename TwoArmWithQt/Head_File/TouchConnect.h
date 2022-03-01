#ifndef _TouchConnect_H
#define _TouchConnect_H

#include <iostream>
#include <windows.h>
#include "chai3d.h"
#include <GLFW/glfw3.h>
#include <Eigen/Dense> 

using chai3d::cHapticDeviceHandler;
using chai3d::cGenericHapticDevicePtr;
using chai3d::cVector3d;
using chai3d::cMatrix3d;
using chai3d::cHapticDeviceInfo;

class TouchConnect{
public:
	int numHapticDevices;				// number of haptic devices detected
	bool simulationRunning;  // a flag to indicate if the haptic simulation currently running
	bool simulationFinished;  // a flag to indicate if the haptic simulation has terminated
	cVector3d firstTouchPosition[2];
	struct TouchData
	{
		bool button0, button1, button2, button3;
		bool UseDamping = false;  // a flag for using damping (ON/OFF)
		bool UseForceField = true;  // a flag for using force field (ON/OFF)

		double gripperAngle;
		double gripperAngularVelocity;
		cVector3d Position;
		cMatrix3d Rotation;
		cVector3d LinearVelocity;
		cVector3d AngularVelocity;
	};
	TouchData CurTouchData[2];
	TouchData PreTouchData[2];

	TouchConnect();
	~TouchConnect();

	//连接Touch设备函数
	void CreateDevice();

	//断开Touch设备函数
	void CloseDevice();

	//更新Touch设备函数
	void UpdateHaptics(int num);

private:
	cHapticDeviceHandler* handler;  // a haptic device handler
	cGenericHapticDevicePtr hapticDevice[2];  // a pointer to the current haptic device
	
};


#endif 
