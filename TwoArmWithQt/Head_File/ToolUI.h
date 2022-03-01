#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <conio.h>//按键触发头文件
#include "rotation.h"
#include "RobotArm.h"
#include "ToolConnect.h"

RobotArm Robot;
ToolConnect ToolConnectCom;

//电动扳手转速
static float  vol1 = 20.0f;

void ToolControlUI(bool ToolControl)
{
	if (ToolControl)
	{
	
		ImGui::Begin(u8"工具控制", &ToolControl);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text(u8"引线抓手");
		ImGui::SameLine();
		if (ImGui::Button(u8"开##1"))                            // 
		{
			ToolConnectCom.command(1, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"关##1"))                            // 
		{
			ToolConnectCom.command(1, 2, 0, 0);
		}

		ImGui::Text(u8"避雷器抓手");
		ImGui::SameLine();
		if (ImGui::Button(u8"开##2"))                            // 
		{
			ToolConnectCom.command(2, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"关##2"))                            // 
		{
			ToolConnectCom.command(2, 2, 0, 0);
		}

		ImGui::Text(u8"电动扳手");								//电动扳手
		ImGui::SameLine();
		if (ImGui::Button(u8"正##1"))                             
		{
			ToolConnectCom.command(3, 1, floor(vol1), 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"反##1"))                            // 
		{
			ToolConnectCom.command(3, 2, floor(vol1), 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"停##1"))                            // 
		{
			ToolConnectCom.command(3, 0, 0, 0);
		}
		ImGui::SameLine();
		ImGui::SliderFloat(u8"Left1", &vol1, 0.0f, 100.0f);

		ImGui::Text(u8"电动液压钳");
		ImGui::SameLine();
		if (ImGui::Button(u8"开##3"))                            // 
		{
			ToolConnectCom.command(4, 2, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"关##3"))                            // 
		{
			ToolConnectCom.command(4, 2, 0, 0);
			Sleep(200);
			ToolConnectCom.command(4, 1, 0, 0);
		}

		ImGui::Text(u8"自动接火装置1");
		ImGui::SameLine();
		if (ImGui::Button(u8"支线夹紧##1"))                            // 
		{
			ToolConnectCom.command(5, 1,0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"支线解锁##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"拧紧线夹##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"停止拧紧##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 0, 2);
		}

		ImGui::Text(u8"自动接火装置2");
		ImGui::SameLine();
		if (ImGui::Button(u8"支线夹紧##2"))                            // 
		{
			ToolConnectCom.command(7, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"支线解锁##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"拧紧线夹##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"停止拧紧##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 0, 2);
		}

		ImGui::Text(u8"自动接火装置3");
		ImGui::SameLine();
		if (ImGui::Button(u8"支线夹紧##3"))                            // 
		{
			ToolConnectCom.command(9, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"支线解锁##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"拧紧线夹##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"停止拧紧##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 0, 2);
		}/**/
		ImGui::Text(u8"剥皮作业前请打开遥控器");
		if (ImGui::Button(u8"开始剥皮"))                            // 
		{
			ToolConnectCom.command(12, 1, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"执行剥皮"))                            // 
		{
			ToolConnectCom.command(12, 2, 0, 10);
			Sleep(10);
			ToolConnectCom.command(12, 1, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"暂停剥皮"))                            // 
		{
			ToolConnectCom.command(12, 2, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"结束剥皮"))                            // 
		{
			ToolConnectCom.command(12, 0, 1, 10);
		}/**/
		if (ImGui::Button(u8"关闭"))
			ToolControl = false;
		ImGui::End();
	}


}