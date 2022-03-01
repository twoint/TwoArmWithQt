//==============================================================================

//==============================================================================
#pragma once
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#ifdef  _WIN64
#pragma warning (disable:4996)
#endif

#if defined(WIN32)


#else
# include "conio.h"
# include <string.h>
#endif

#if defined(WIN32) || defined(linux)
//#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#endif


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>     
#include <string>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "UI_Design.h"


using std::ostringstream;


//------------------------------------------------------------------------------
#include "chai3d.h"
//------------------------------------------------------------------------------
//#include <GLFW/glfw3.h>

//------------------------------------------------------------------------------
using namespace chai3d;
//using namespace std;
//------------------------------------------------------------------------------


//==============================================================================

DWORD WINAPI UIThread(LPVOID lpParam)
{
	Start_UI_Interface();
	while (!glfwWindowShouldClose(window))
	{
		main_graphic_loop();
		/*	OutFile << CurVrepData.RecvLeftJoint[0] << " " << CurVrepData.RecvLeftJoint[1] << " " << CurVrepData.RecvLeftJoint[2] << " "
				<< CurVrepData.RecvLeftJoint[3] << " " << CurVrepData.RecvLeftJoint[4] << " " << CurVrepData.RecvLeftJoint[5] << "\n";
	*/
	}
	close_UI_Interface();
	return 0;
}

int main(int argc, char* argv[])
{

	
	//Receive5 task5;//计时
	//task5.Run();
	
	ToolConnectCom.OpenCom("\\\\.\\COM16", 19200, 0, 8, 1);
	
	
	Robot.StartUp();

	DWORD threadID5;//UI界面
	HANDLE hThread5;
	//hThread5 = CreateThread(NULL, 0, InterfaceThread, NULL, 0, &threadID5);
	
	Start_UI_Interface();
	while (!glfwWindowShouldClose(window))
	{
		main_graphic_loop();
	}
	close_UI_Interface();

	return (0);
}
