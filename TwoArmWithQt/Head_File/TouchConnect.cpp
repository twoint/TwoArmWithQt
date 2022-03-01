#include "TouchConnect.h"

using namespace std;

TouchConnect::TouchConnect(){
	this->numHapticDevices = 0;
	this->simulationRunning = false;
	this->simulationFinished = true;
	this->handler = NULL;
}

TouchConnect::~TouchConnect(){
	this->CloseDevice();
	delete this->handler;
}

void TouchConnect::CreateDevice() {
	// create a haptic device handler
	this->handler = new cHapticDeviceHandler();

	// get number of haptic devices
	this->numHapticDevices = this->handler->getNumDevices();

	// setup each haptic device
	for (int i = 0; i < this->numHapticDevices; i++)
	{
		// get a handle to the first haptic device
		this->handler->getDevice(this->hapticDevice[i], i);

		// open a connection to haptic device
		this->hapticDevice[i]->open();

		// calibrate device (if necessary)
		this->hapticDevice[i]->calibrate();

		// retrieve information about the current haptic device
		cHapticDeviceInfo info = this->hapticDevice[i]->getSpecifications();

		// if the device has a gripper, enable the gripper to simulate a user switch
		this->hapticDevice[i]->setEnableGripperUserSwitch(true);

	}
}

void TouchConnect::CloseDevice() {
	// stop the simulation
	this->simulationRunning = false;

	// wait for graphics and haptics loops to terminate
	while (!this->simulationFinished) { Sleep(100); }

	// close haptic device
	for (int i = 0; i < this->numHapticDevices; i++)
	{
		this->hapticDevice[i]->close();
	}
}

void TouchConnect::UpdateHaptics(int num)
{
	// read position 
	cVector3d position;
	this->hapticDevice[num]->getPosition(position);
	this->CurTouchData[num].Position = position;		// update global variable for graphic display update
	// read orientation 
	cMatrix3d rotation;
	this->hapticDevice[num]->getRotation(rotation);
	this->CurTouchData[num].Rotation = rotation;		// update global variable for graphic display update
	// read gripper position
	double gripperAngle;
	this->hapticDevice[num]->getGripperAngleRad(gripperAngle);
	this->CurTouchData[num].gripperAngle = gripperAngle;
	// read linear velocity 
	cVector3d linearVelocity;
	this->hapticDevice[num]->getLinearVelocity(linearVelocity);
	this->CurTouchData[num].LinearVelocity = linearVelocity;
	// read angular velocity
	cVector3d angularVelocity;
	this->hapticDevice[num]->getAngularVelocity(angularVelocity);
	this->CurTouchData[num].AngularVelocity = angularVelocity;
	// read gripper angular velocity
	double gripperAngularVelocity;
	this->hapticDevice[num]->getGripperAngularVelocity(gripperAngularVelocity);
	this->CurTouchData[num].gripperAngularVelocity = gripperAngularVelocity;
	// read user-switch status (button 0)

	this->CurTouchData[num].button0 = false;
	this->CurTouchData[num].button1 = false;
	this->CurTouchData[num].button2 = false;
	this->CurTouchData[num].button3 = false;

	this->hapticDevice[num]->getUserSwitch(0, this->CurTouchData[num].button0);
	this->hapticDevice[num]->getUserSwitch(1, this->CurTouchData[num].button1);
	this->hapticDevice[num]->getUserSwitch(2, this->CurTouchData[num].button2);
	this->hapticDevice[num]->getUserSwitch(3, this->CurTouchData[num].button3);

	/////////////////////////////////////////////////////////////////////
	// COMPUTE AND APPLY FORCES
	/////////////////////////////////////////////////////////////////////
	// desired position
	cVector3d desiredPosition;
	desiredPosition.set(0.0, 0.0, 0.0);

	// desired orientation
	cMatrix3d desiredRotation;
	desiredRotation.identity();

	// variables for forces
	cVector3d force(0, 0, 0);
	cVector3d torque(0, 0, 0);
	double gripperForce = 0.0;

	// apply force field
	if (this->CurTouchData[num].UseForceField)
	{
		// compute linear force
		double Kp = 25; // [N/m]
		cVector3d forceField = Kp * (desiredPosition - position);
		force.add(forceField);

		// compute angular torque
		double Kr = 0.05; // [N/m.rad]
		cVector3d axis;
		double angle;
		cMatrix3d deltaRotation = cTranspose(rotation) * desiredRotation;
		deltaRotation.toAxisAngle(axis, angle);
		torque = rotation * ((Kr * angle) * axis);
	}

	// apply damping term
	if (this->CurTouchData[num].UseDamping)
	{
		cHapticDeviceInfo info = hapticDevice[num]->getSpecifications();

		// compute linear damping force
		double Kv = 1.0 * info.m_maxLinearDamping;
		cVector3d forceDamping = -Kv * linearVelocity;
		force.add(forceDamping);

		// compute angular damping force
		double Kvr = 1.0 * info.m_maxAngularDamping;
		cVector3d torqueDamping = -Kvr * angularVelocity;
		torque.add(torqueDamping);

		// compute gripper angular damping force
		double Kvg = 1.0 * info.m_maxGripperAngularDamping;
		gripperForce = gripperForce - Kvg * gripperAngularVelocity;
	}

	// send computed force, torque, and gripper force to haptic device
	this->hapticDevice[num]->setForceAndTorqueAndGripperForce(force, torque, gripperForce);

}
