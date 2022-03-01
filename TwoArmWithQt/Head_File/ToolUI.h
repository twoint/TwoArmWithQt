#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include <conio.h>//��������ͷ�ļ�
#include "rotation.h"
#include "RobotArm.h"
#include "ToolConnect.h"

RobotArm Robot;
ToolConnect ToolConnectCom;

//�綯����ת��
static float  vol1 = 20.0f;

void ToolControlUI(bool ToolControl)
{
	if (ToolControl)
	{
	
		ImGui::Begin(u8"���߿���", &ToolControl);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text(u8"����ץ��");
		ImGui::SameLine();
		if (ImGui::Button(u8"��##1"))                            // 
		{
			ToolConnectCom.command(1, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"��##1"))                            // 
		{
			ToolConnectCom.command(1, 2, 0, 0);
		}

		ImGui::Text(u8"������ץ��");
		ImGui::SameLine();
		if (ImGui::Button(u8"��##2"))                            // 
		{
			ToolConnectCom.command(2, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"��##2"))                            // 
		{
			ToolConnectCom.command(2, 2, 0, 0);
		}

		ImGui::Text(u8"�綯����");								//�綯����
		ImGui::SameLine();
		if (ImGui::Button(u8"��##1"))                             
		{
			ToolConnectCom.command(3, 1, floor(vol1), 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"��##1"))                            // 
		{
			ToolConnectCom.command(3, 2, floor(vol1), 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"ͣ##1"))                            // 
		{
			ToolConnectCom.command(3, 0, 0, 0);
		}
		ImGui::SameLine();
		ImGui::SliderFloat(u8"Left1", &vol1, 0.0f, 100.0f);

		ImGui::Text(u8"�綯Һѹǯ");
		ImGui::SameLine();
		if (ImGui::Button(u8"��##3"))                            // 
		{
			ToolConnectCom.command(4, 2, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"��##3"))                            // 
		{
			ToolConnectCom.command(4, 2, 0, 0);
			Sleep(200);
			ToolConnectCom.command(4, 1, 0, 0);
		}

		ImGui::Text(u8"�Զ��ӻ�װ��1");
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߼н�##1"))                            // 
		{
			ToolConnectCom.command(5, 1,0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߽���##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"š���߼�##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"ֹͣš��##1"))                            // 
		{
			ToolConnectCom.command(5, 0, 0, 2);
		}

		ImGui::Text(u8"�Զ��ӻ�װ��2");
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߼н�##2"))                            // 
		{
			ToolConnectCom.command(7, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߽���##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"š���߼�##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"ֹͣš��##2"))                            // 
		{
			ToolConnectCom.command(7, 0, 0, 2);
		}

		ImGui::Text(u8"�Զ��ӻ�װ��3");
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߼н�##3"))                            // 
		{
			ToolConnectCom.command(9, 1, 0, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"֧�߽���##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 1, 0);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"š���߼�##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 0, 1);
		}
		ImGui::SameLine();
		if (ImGui::Button(u8"ֹͣš��##3"))                            // 
		{
			ToolConnectCom.command(9, 0, 0, 2);
		}/**/
		ImGui::Text(u8"��Ƥ��ҵǰ���ң����");
		if (ImGui::Button(u8"��ʼ��Ƥ"))                            // 
		{
			ToolConnectCom.command(12, 1, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"ִ�а�Ƥ"))                            // 
		{
			ToolConnectCom.command(12, 2, 0, 10);
			Sleep(10);
			ToolConnectCom.command(12, 1, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"��ͣ��Ƥ"))                            // 
		{
			ToolConnectCom.command(12, 2, 0, 10);
		}/**/
		ImGui::SameLine();
		if (ImGui::Button(u8"������Ƥ"))                            // 
		{
			ToolConnectCom.command(12, 0, 1, 10);
		}/**/
		if (ImGui::Button(u8"�ر�"))
			ToolControl = false;
		ImGui::End();
	}


}