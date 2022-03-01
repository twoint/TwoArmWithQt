#pragma once
 
#include "CreateTexture.h"
#include <windows.h>
#include <vector>

#define LongArm_X_Offset  0.0 
#define LongArm_Y_Offset   0 
#define LongArm_Z_Offset  0.0
#define ShortArm_X_Offset  0 
#define ShortArm_Y_Offset  0
#define ShortArm_Z_Offset  0

// a flag for using damping (ON/OFF)
GLFWwindow* window;
// Our state
bool show_demo_window = false;
bool Left_Arm_window = false;
bool Right_Arm_window = false;
bool WetCamera_window = true;
bool Vrep_window = true;
bool ToolControl = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

static int CounterBotton[9] = { 0,0,0,0,0,0,0,0,0};
bool key_w = 0;
bool useDamping = false;
bool num_c3 = 0;

//GLuint IconID;
//GLuint ImageID;
//GLuint CameraID;
//GLuint VrepID;
cv::VideoCapture vcap;
const std::string videoStreamAddress = "rtsp://192.168.1.193:554/user=admin_password=tlJwpbo6_channel=0_stream=0.sdp?real_stream";
bool FlagWetCamera = false;

static float  LeftValue = 1.0f;		//左臂示教比例
static float  RightValue = 5.0f;		//右臂示教比例
bool Teach_pendant = false;
int toolID = 7;


//声明
void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);
void TeachUI();
void JieHuoUI();
void GuZhangZhiShiQiUI();
//GLuint ImageCreateTexture(const String& m_fileName);
//void CameraCreateTexture(cv::VideoCapture vcap);
//void VrepCreateTexture(simxUChar* vrep_image);

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int Start_UI_Interface()
{

	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	window = glfwCreateWindow(1600, 900, u8" 配网带电作业机器人遥操作控制系统", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
   // ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL2_Init();

	io.Fonts->AddFontFromFileTTF("font/ShangShouJianHeiXianXiTi-2.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

}

void new_graphic() {
	//功能图标参数
	int frame_padding = 3;                             // -1 == uses default padding (style.FramePadding)
	ImVec2 size = ImVec2(220.0f, 140.0f);                     // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint

	//工具图标参数
	ImVec2 size2 = ImVec2(140.0f, 100.0f);                     // Size of the image we want to make visible
	//打开关闭图标参数
	ImVec2 size3 = ImVec2(20.0f, 20.0f);                     // Size of the image we want to make visible
	//方向图标参数
	ImVec2 size4 = ImVec2(112.0f, 140.0f);                     // Size of the image we want to make visible

	//1/1功能内部图片
	ImVec2 size5 = ImVec2(480.0f, 40.0f);                     // Size of the image we want to make visible
	//1/2功能内部图片
	ImVec2 size6 = ImVec2(232.0f, 40.0f);                     // Size of the image we want to make visible
	//1/4功能内部图片
	ImVec2 size7 = ImVec2(108.0f, 40.0f);                     // Size of the image we want to make visible

	textureInit();

	//电网logo显示
	ImGui::Image((GLuint*)ImageID, ImVec2(500, 150));

	//控制模块
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"控制");

	ImGui::Checkbox(u8"示教器信息", &Teach_pendant);

	//ImGui::SameLine();
	if (!Robot.LeftInf.Connect)
	{
		//ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"左臂未连接");

	}
	if (!Robot.LeftInf.Brake)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"左臂未开启抱闸");

	}
	if (!Robot.LeftInf.Script)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"左臂脚本错误");

	}

	if (!Robot.RightInf.Connect)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"右臂未连接");

	}

	TeachUI();

	ImGui::BeginChild(u8"工具", ImVec2(500, 750), true);
	{
		if (ImGui::BeginTabBar("tools", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(u8"工具开关"))
			{
				//清同步标志位
				//LeftTeleoperationFlag = false;
				//RightTeleoperationFlag = false;
				//左臂网络开关
				static bool internet_left_tag = false;
				ImTextureID internet_left_tag_id;
				if (internet_left_tag == false)
					internet_left_tag_id = logo07_Id;
				else
					internet_left_tag_id = logo08_Id;
				if (ImGui::ImageButton(internet_left_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					internet_left_tag = !internet_left_tag;
					if (internet_left_tag == false)
					{
						Robot.CloseSocket();//断开网络
					}
					else if (internet_left_tag == true)
					{
						Robot.CreatSocket("192.168.1.100", 8010);//连接网络
					}
				}
				ImGui::SameLine();

				//右臂网络开关
				static bool internet_right_tag = false;
				ImTextureID internet_right_tag_id;
				if (internet_right_tag == false)
					internet_right_tag_id = logo09_Id;
				else
					internet_right_tag_id = logo10_Id;
				if (ImGui::ImageButton(internet_right_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					internet_right_tag = !internet_right_tag;
					if (internet_right_tag == false)
					{
						Robot.CloseCom();//断开网络
					}
					else if (internet_right_tag == true)
					{
						Robot.OpenCom("COM1", 115200, 0, 8, 1);//连接网络
					}
				}
				ImGui::SameLine();

				//引线抓手开关
				static bool line_grip_tag = false;
				ImTextureID line_grip_tag_id;
				if (line_grip_tag == false)
					line_grip_tag_id = logo11_Id;
				else
					line_grip_tag_id = logo12_Id;
				if (ImGui::ImageButton(line_grip_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					line_grip_tag = !line_grip_tag;
					if (line_grip_tag == false)
					{
						ToolConnectCom.command(2, 1, 0, 0);
					}
					else if (line_grip_tag == true)
					{
						ToolConnectCom.command(2, 2, 0, 0);
					}
				}

				//避雷器抓手开关
				static bool lightning_protection_tag = false;
				ImTextureID lightning_protection_tag_id;
				if (lightning_protection_tag == false)
					lightning_protection_tag_id = logo13_Id;
				else
					lightning_protection_tag_id = logo14_Id;
				if (ImGui::ImageButton(lightning_protection_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					lightning_protection_tag = !lightning_protection_tag;
					if (lightning_protection_tag == false)
					{
						ToolConnectCom.command(1, 1, 0, 0);//避雷器抓手开

					}
					else if (lightning_protection_tag == true)
					{
						ToolConnectCom.command(1, 2, 0, 0);//避雷器抓手关
					}
				}
				ImGui::SameLine();

				//电动扳手正转开关
				static float  vol = 20.0f;		//电动扳手转速
				static bool electric_wrench_forward_tag = false;
				ImTextureID electric_wrench_forward_tag_id;
				if (electric_wrench_forward_tag == false)
					electric_wrench_forward_tag_id = logo15_Id;
				else
					electric_wrench_forward_tag_id = logo16_Id;
				if (ImGui::ImageButton(electric_wrench_forward_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					electric_wrench_forward_tag = !electric_wrench_forward_tag;
					if (electric_wrench_forward_tag == false)
					{
						ToolConnectCom.command(3, 0, 0, 0);//停
					}
					else if (electric_wrench_forward_tag == true)
					{
						ToolConnectCom.command(3, 1, floor(vol), 0);//正转
					}
				}
				ImGui::SameLine();

				//电动扳手反转开关
				static bool electric_wrench_reverse_tag = false;
				ImTextureID electric_wrench_reverse_tag_id;
				if (electric_wrench_reverse_tag == false)
					electric_wrench_reverse_tag_id = logo17_Id;
				else
					electric_wrench_reverse_tag_id = logo18_Id;
				if (ImGui::ImageButton(electric_wrench_reverse_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					electric_wrench_reverse_tag = !electric_wrench_reverse_tag;
					if (electric_wrench_reverse_tag == false)
					{
						ToolConnectCom.command(3, 0, 0, 0);//停
					}
					else if (electric_wrench_reverse_tag == true)
					{
						ToolConnectCom.command(3, 2, floor(vol), 0);//反转
					}
				}

				//电动液压钳开关
				static bool electric_hydraulic_clamp_tag = false;
				ImTextureID electric_hydraulic_clamp_tag_id;
				if (electric_hydraulic_clamp_tag == false)
					electric_hydraulic_clamp_tag_id = logo19_Id;
				else
					electric_hydraulic_clamp_tag_id = logo20_Id;
				if (ImGui::ImageButton(electric_hydraulic_clamp_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					electric_hydraulic_clamp_tag = !electric_hydraulic_clamp_tag;
					if (electric_hydraulic_clamp_tag == false)
					{
						ToolConnectCom.command(4, 2, 0, 0);
					}
					else if (electric_hydraulic_clamp_tag == true)
					{
						ToolConnectCom.command(4, 2, 0, 0);
						Sleep(200);
						ToolConnectCom.command(4, 1, 0, 0);
					}
				}
				ImGui::SameLine();

				//自动接火装置1支线夹紧
				static bool automatic_ignition_device_clamping_1_tag = false;
				ImTextureID automatic_ignition_device_clamping_1_tag_id;
				if (automatic_ignition_device_clamping_1_tag == false)
					automatic_ignition_device_clamping_1_tag_id = logo21_Id;
				else
					automatic_ignition_device_clamping_1_tag_id = logo22_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_1_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_clamping_1_tag = !automatic_ignition_device_clamping_1_tag;
					if (automatic_ignition_device_clamping_1_tag == false)
					{
						ToolConnectCom.command(5, 0, 1, 0);//解锁
					}
					else if (automatic_ignition_device_clamping_1_tag == true)
					{
						ToolConnectCom.command(5, 1, 0, 0);//夹紧
					}
				}
				ImGui::SameLine();

				//自动接火装置1拧紧线夹
				static bool automatic_ignition_device_tighten_1_tag = false;
				ImTextureID automatic_ignition_device_tighten_1_tag_id;
				if (automatic_ignition_device_tighten_1_tag == false)
					automatic_ignition_device_tighten_1_tag_id = logo23_Id;
				else
					automatic_ignition_device_tighten_1_tag_id = logo24_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_1_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_tighten_1_tag = !automatic_ignition_device_tighten_1_tag;
					if (automatic_ignition_device_tighten_1_tag == false)
					{
						ToolConnectCom.command(5, 0, 0, 2);//停止拧紧
					}
					else if (automatic_ignition_device_tighten_1_tag == true)
					{
						ToolConnectCom.command(5, 0, 0, 1);//拧紧线夹
					}
				}

				//自动接火装置2支线夹紧
				static bool automatic_ignition_device_clamping_2_tag = false;
				ImTextureID automatic_ignition_device_clamping_2_tag_id;
				if (automatic_ignition_device_clamping_2_tag == false)
					automatic_ignition_device_clamping_2_tag_id = logo25_Id;
				else
					automatic_ignition_device_clamping_2_tag_id = logo26_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_2_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_clamping_2_tag = !automatic_ignition_device_clamping_2_tag;
					if (automatic_ignition_device_clamping_2_tag == false)
					{
						ToolConnectCom.command(7, 0, 1, 0);//解锁
					}
					else if (automatic_ignition_device_clamping_2_tag == true)
					{
						ToolConnectCom.command(7, 1, 0, 0);//夹紧
					}
				}
				ImGui::SameLine();

				//自动接火装置2拧紧线夹
				static bool automatic_ignition_device_tighten_2_tag = false;
				ImTextureID automatic_ignition_device_tighten_2_tag_id;
				if (automatic_ignition_device_tighten_2_tag == false)
					automatic_ignition_device_tighten_2_tag_id = logo27_Id;
				else
					automatic_ignition_device_tighten_2_tag_id = logo28_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_2_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_tighten_2_tag = !automatic_ignition_device_tighten_2_tag;
					if (automatic_ignition_device_tighten_2_tag == false)
					{
						ToolConnectCom.command(7, 0, 0, 2);//停止拧紧
					}
					else if (automatic_ignition_device_tighten_2_tag == true)
					{
						ToolConnectCom.command(7, 0, 0, 1);//拧紧线夹
					}
				}
				ImGui::SameLine();

				//自动接火装置3支线夹紧
				static bool automatic_ignition_device_clamping_3_tag = false;
				ImTextureID automatic_ignition_device_clamping_3_tag_id;
				if (automatic_ignition_device_clamping_3_tag == false)
					automatic_ignition_device_clamping_3_tag_id = logo29_Id;
				else
					automatic_ignition_device_clamping_3_tag_id = logo30_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_3_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_clamping_3_tag = !automatic_ignition_device_clamping_3_tag;
					if (automatic_ignition_device_clamping_3_tag == false)
					{
						ToolConnectCom.command(9, 0, 1, 0);//解锁
					}
					else if (automatic_ignition_device_clamping_3_tag == true)
					{
						ToolConnectCom.command(9, 1, 0, 0);//夹紧
					}
				}

				//自动接火装置3拧紧线夹
				static bool automatic_ignition_device_tighten_3_tag = false;
				ImTextureID automatic_ignition_device_tighten_3_tag_id;
				if (automatic_ignition_device_tighten_3_tag == false)
					automatic_ignition_device_tighten_3_tag_id = logo31_Id;
				else
					automatic_ignition_device_tighten_3_tag_id = logo32_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_3_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					automatic_ignition_device_tighten_3_tag = !automatic_ignition_device_tighten_3_tag;
					if (automatic_ignition_device_tighten_3_tag == false)
					{
						ToolConnectCom.command(9, 0, 0, 2);//停止拧紧
					}
					else if (automatic_ignition_device_tighten_3_tag == true)
					{
						ToolConnectCom.command(9, 0, 0, 1);//拧紧线夹
					}
				}
				ImGui::SameLine();

				//左臂映射开关
				static bool left_arm_mapping_tag = false;
				ImTextureID left_arm_mapping_tag_id;
				if (left_arm_mapping_tag == false)
					left_arm_mapping_tag_id = logo33_Id;
				else
					left_arm_mapping_tag_id = logo34_Id;
				if (ImGui::ImageButton(left_arm_mapping_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					left_arm_mapping_tag = !left_arm_mapping_tag;
					if (left_arm_mapping_tag == false)
					{
						Robot.LeftInf.Sync = 0;//左臂映射关
					}
					else if (left_arm_mapping_tag == true)
					{
						Robot.LeftInf.Sync = 1;//左臂映射开
					}
				}
				ImGui::SameLine();

				//右臂映射开关
				static bool right_arm_mapping_tag = false;
				ImTextureID right_arm_mapping_tag_id;
				if (right_arm_mapping_tag == false)
					right_arm_mapping_tag_id = logo35_Id;
				else
					right_arm_mapping_tag_id = logo36_Id;
				if (ImGui::ImageButton(right_arm_mapping_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("点击按钮了\n");
					right_arm_mapping_tag = !right_arm_mapping_tag;
					if (right_arm_mapping_tag == false)
					{
						Robot.RightInf.Sync = 0;//右臂映射关
					}
					else if (right_arm_mapping_tag == true)
					{
						Robot.RightInf.Sync = 1;//右臂映射开
					}
				}

				ImGui::SliderFloat("  ", &vol, 0.0f, 100.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"扭矩");

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"左臂遥操作"))
			{
				{
					float Angle_Dis[6] = { 0,0,0,0,0,0 };
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向前
					if (ImGui::ImageButton(direction_forward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[1] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向上
					if (ImGui::ImageButton(direction_up_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[2] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					/*************下一行****************/
					//向左
					if (ImGui::ImageButton(direction_left_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[0] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向右
					if (ImGui::ImageButton(direction_right_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[0] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					/*************下一行****************/
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向后
					if (ImGui::ImageButton(direction_backward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[1] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向下
					if (ImGui::ImageButton(direction_down_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[2] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					//ImGui::SameLine();
				}
				ImGui::SliderFloat("  ", &LeftValue, 0.1f, 5.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"左示教比例");
				//ImGui::EndChild();				
				ImGui::EndTabItem();

			}

			if (ImGui::BeginTabItem(u8"右臂遥操作"))
			{
				{
					vector<float> AngleDif(6, 0);
						//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向前
					if (ImGui::ImageButton(direction_forward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[1] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向上
					ImGui::PushButtonRepeat(true);
					if (ImGui::ImageButton(direction_up_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
						//if (ImGui::Button(u8"右臂发送数据"))
					{
						AngleDif[2] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::PopButtonRepeat();
					/*************下一行****************/
					//向左
					if (ImGui::ImageButton(direction_left_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[0] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向右
					if (ImGui::ImageButton(direction_right_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[0] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					/*************下一行****************/
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向后
					if (ImGui::ImageButton(direction_backward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[1] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//占位
					ImGui::Text("			");
					ImGui::SameLine();
					//向下
					if (ImGui::ImageButton(direction_down_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[2] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
				}

				/*ImGui::SameLine();*/
				ImGui::SliderFloat("  ", &RightValue, 0.1f, 10.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"右示教比例");
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndChild();

	//下一格
	ImGui::NextColumn();

	//九项作业模块
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"九项作业");
	static bool work_initialize_tag = true;        //显示九项作业时为true
	static bool function01_tag = false;            //运行作业1此标志为true
	static bool function02_tag = false;            //运行作业2此标志为true
	static bool function03_tag = false;            //运行作业3此标志为true
	static bool function04_tag = false;            //运行作业4此标志为true
	static bool function05_tag = false;            //运行作业5此标志为true
	static bool function06_tag = false;            //运行作业6此标志为true
	static bool function07_tag = false;            //运行作业7此标志为true
	static bool function08_tag = false;            //运行作业8此标志为true
	static bool function09_tag = false;            //运行作业9此标志为true
	static bool enter_main_tag = true;			   //为了消除抖动bug，为true时显示作业主界面
	ImGui::BeginChild(u8"作业", ImVec2(500, 900), true);
	{
		enter_main_tag = true;
		//进入接支接线路引线作业
		if (function01_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"接支接线路引线   换场景");
			JieHuoUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function01_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入断支接线路引线作业
		if (function02_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"断支接线路引线   避雷器场景");
			//JianXianUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function02_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入更换避雷器作业
		if (function03_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"更换避雷器");
			//BiLeiQiUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function03_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入接跌落式熔断器上引线作业
		if (function04_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"接跌落式熔断器上引线");
			//JieHuoUI();

			if (ImGui::Button(u8"返回九项作业"))
			{
				function04_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入断跌落式熔断器上引线作业
		if (function05_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"断跌落式熔断器上引线");
			//JianXianUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function05_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入更换跌落式熔断器作业
		if (function06_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"更换跌落式熔断器");
			//BaoXianQiUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function06_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入装故障指示器作业
		if (function07_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"装故障指示器");
			//GuZhangZhiShiQiUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function07_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入拆故障指示器作业
		if (function08_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"拆故障指示器");
			GuZhangZhiShiQiUI();

			if (ImGui::Button(u8"返回九项作业"))
			{
				function08_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//进入除枝作业
		if (function09_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"除枝");
			//JianXianUI();
			if (ImGui::Button(u8"返回九项作业"))
			{
				function09_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}

		//显示九项作业主页
		if (work_initialize_tag == true && enter_main_tag == true)
		{
			if (ImGui::ImageButton(function_logo_01_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function01_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_02_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function02_tag = true;
			}

			if (ImGui::ImageButton(function_logo_03_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function03_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_04_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function04_tag = true;
			}

			if (ImGui::ImageButton(function_logo_05_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function05_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_06_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function06_tag = true;
			}

			if (ImGui::ImageButton(function_logo_07_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function07_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_08_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function08_tag = true;
			}

			if (ImGui::ImageButton(function_logo_09_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("点击按钮了\n");
				work_initialize_tag = false;
				function09_tag = true;
			}
		}

	}
	ImGui::EndChild();

	//下一格
	ImGui::NextColumn();

	//网络相机
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"相机");
	ImGui::BeginChild(u8"网络相机", ImVec2(500, 370), true);
	{
		if (ImGui::BeginTabBar("video", ImGuiTabBarFlags_None))
		{
			//主相机显示
			if (ImGui::BeginTabItem(u8"主相机"))
			{
				static bool rtspFlag1 = false;

				if (ImGui::Button(u8"打开"))
				{
					FlagWetCamera = vcap.open(videoStreamAddress);
					if (!rtspFlag1)
					{
						rtspFlag1 = true;
						glGenTextures(1, &CameraID);
					}
				}

				ImGui::SameLine();
				if (ImGui::Button(u8"关闭"))
				{
					vcap.release();
					FlagWetCamera = false;
					rtspFlag1 = false;
				}

				if (FlagWetCamera)
				{
					CameraCreateTexture(vcap, CameraID);
					ImGui::Image((GLuint*)CameraID, ImVec2(480, 290));
				}
				if (!FlagWetCamera)
				{
					ImGui::Image((GLuint*)CameraCloseImageID, ImVec2(480, 290));
				}
				ImGui::EndTabItem();

			}
			//剪线相机显示
			if (ImGui::BeginTabItem(u8"剪线相机"))
			{
				
			}
			//保险器相机显示
			if (ImGui::BeginTabItem(u8"保险器相机"))
			{

			}

		}

	}
	ImGui::EndChild();


	//Vrep图像
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"vrep图像");
	ImGui::BeginChild(u8"Vrep图像", ImVec2(500, 500), true);
	{
		//vrep图标显示
		//ImGui::Image((GLuint*)VrepLogoID, ImVec2(500, 30));
		static bool vrepFlag = false;
		if (ImGui::Button(u8"打开"))
		{
			Robot.FlagVrepImg = true;
			if (!vrepFlag)
			{
				vrepFlag = true;
				glGenTextures(0, &VrepID);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button(u8"关闭"))
		{
			vrepFlag = false;
			Robot.FlagVrepImg = false;
		}

		if (Robot.FlagVrepImg)
		{
			VrepCreateTexture(Robot.vrep_image);
			ImGui::Image((GLuint*)VrepID, ImVec2(480, 450));
		}
		if (!Robot.FlagVrepImg)
		{
			ImGui::Image((GLuint*)CameraCloseImageID, ImVec2(480, 290));
		}

		ImGui::EndChild();
	}
}

void main_graphic_loop()
{
	// set key callback
	glfwSetKeyCallback(window, keyCallback);
	glfwPollEvents();

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// //设置窗口的padding为0是图片控件充满窗口
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	//设置窗口的边框大小为0
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

	ImGui::StyleColorsBlue();


	bool start_tag = true;
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin(u8"机器人信息", &start_tag, 1 << 10);
	ImGui::Columns(3, 0, false);
	ImGui::Separator();

	//内容
	//显示新界面
	new_graphic();

	ImGui::End();


	// Rendering
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	// If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!), 
	// you may need to backup/reset/restore current shader using the commented lines below.
	//GLint last_program; 
	//glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	//glUseProgram(0);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	//glUseProgram(last_program);

	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void close_UI_Interface()
{
// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	// filter calls that only include a key press
	if ((a_action != GLFW_PRESS) && (a_action != GLFW_REPEAT))
	{
		return;
	}

	// option - exit
	if ((a_key == GLFW_KEY_ESCAPE) || (a_key == GLFW_KEY_Q))
	{
		glfwSetWindowShouldClose(a_window, GLFW_TRUE);
	}

	if (a_key == GLFW_KEY_O)
	{
		useDamping = !useDamping;
		if (useDamping)
			cout << "> Enable damping         \r";
		else
			cout << "> Disable damping        \r";
	}

}

void TeachUI()
{
	if (Teach_pendant)
	{

		ImGui::Begin(u8"Robot Imformation", &Teach_pendant);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		static float f0 = 1.0f;
		ImGui::InputFloat("input K", &f0, 0.01f, 1.0f, "%.3f");
		ImGui::Text(u8"Left Arm Imformation");
		ImGui::PushButtonRepeat(true);
		
		//左臂角度显示
		{
			ImGui::LabelText(u8"灵巧臂角度", u8"     操控");
			float A1_LeftRobot = Robot.LeftInf.Angle[0];
			float A2_LeftRobot = Robot.LeftInf.Angle[1];
			float A3_LeftRobot = Robot.LeftInf.Angle[2];
			float A4_LeftRobot = Robot.LeftInf.Angle[3];
			float A5_LeftRobot = Robot.LeftInf.Angle[4];
			float A6_LeftRobot = Robot.LeftInf.Angle[5];
			float Rel_LeftRobot = 0.017f;
			float Angle_Dis[6] = { 0,0,0,0,0,0 };

			if (ImGui::ArrowButton("A1_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[0] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 1, Robot.LeftInf.Angle[0]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A1_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[0] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}

			
			if (ImGui::ArrowButton("A2_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[1] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 2, Robot.LeftInf.Angle[1]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A2_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[1] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}


			if (ImGui::ArrowButton("A3_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[2] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 3, Robot.LeftInf.Angle[2]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A3_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[2] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}

			if (ImGui::ArrowButton("A4_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[3] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 4, Robot.LeftInf.Angle[3]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A4_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[3] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}

			if (ImGui::ArrowButton("A5_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[4] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 5, Robot.LeftInf.Angle[4]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A5_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[4] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}

			if (ImGui::ArrowButton("A6_LeftRobot_Dec", ImGuiDir_Left))
			{
				Angle_Dis[5] = -Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 5, Robot.LeftInf.Angle[5]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A6_LeftRobot_Inc", ImGuiDir_Right))
			{
				Angle_Dis[5] = Rel_LeftRobot;
				Robot.Set(LeftArm, PTPRel_Angle, Angle_Dis);
			}
		}
		ImGui::NewLine();
		ImGui::Separator();
		//右臂角度显示
		{
			ImGui::LabelText(u8"液压臂角度", u8"     操控");
			float A1_RightRobot = Robot.RightInf.Angle[0];
			float A2_RightRobot = Robot.RightInf.Angle[1];
			float A3_RightRobot = Robot.RightInf.Angle[2];
			float A4_RightRobot = Robot.RightInf.Angle[3];
			float A5_RightRobot = Robot.RightInf.Angle[4];
			float A6_RightRobot = Robot.RightInf.Angle[5];
			float Rel_RightRobot = 0.017f;
			vector<float> AngleDif(6, 0);
			
			if (ImGui::ArrowButton("A1_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[0] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 1, Robot.RightInf.Angle[0]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A1_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[0] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}

			if (ImGui::ArrowButton("A2_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[1] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 2, Robot.RightInf.Angle[1]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A2_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[1] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}

			if (ImGui::ArrowButton("A3_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[2] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 3, Robot.RightInf.Angle[2]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A3_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[2] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}

			if (ImGui::ArrowButton("A4_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[3] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 4, Robot.RightInf.Angle[3]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A4_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[3] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			
			if (ImGui::ArrowButton("A5_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[4] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 5, Robot.RightInf.Angle[4]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A5_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[4] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}

			if (ImGui::ArrowButton("A6_RightRobot_Dec", ImGuiDir_Left))
			{
				AngleDif[5] = -Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
			ImGui::SameLine();
			ImGui::Text("Joint %d:		%.2f		", 6, Robot.RightInf.Angle[5]);
			ImGui::SameLine();
			if (ImGui::ArrowButton("A6_RightRobot_Inc", ImGuiDir_Right))
			{
				AngleDif[5] = Rel_RightRobot;
				Robot.Set(RightArm, PTPRel_Angle, &AngleDif[0]);
			}
		}
		ImGui::NewLine();
		ImGui::Separator();

		ImGui::PopButtonRepeat();

		ImGui::End();
	}


}

void JieHuoUI()
{

	if (ImGui::Button(u8"左臂打开抱闸"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, OpenBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂关闭抱闸"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, CloseBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂重启脚本"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, RestartScrip);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂映射##1"))                            // 
	{
		Robot.LeftInf.Sync = !Robot.LeftInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.LeftInf.Sync == 1)
	{
		ImGui::Text(u8"左映射");
	}
	else
	{
		ImGui::Text(u8"左不映射");
	}

	if (ImGui::Button(u8"右臂映射##1"))                            // 
	{
		Robot.RightInf.Sync = !Robot.RightInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.RightInf.Sync == 1)
	{
		ImGui::Text(u8"右映射");
	}
	else
	{
		ImGui::Text(u8"右不映射");
	}

	if (ImGui::Button(u8"			停止机器人记录点		")) // 
	{
		Robot.Set(LeftArm, StopRobot);
		printf("左臂角度:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.LeftInf.Angle[0], Robot.LeftInf.Angle[1], Robot.LeftInf.Angle[2], Robot.LeftInf.Angle[3], Robot.LeftInf.Angle[4], Robot.LeftInf.Angle[5]);
		printf("右臂角度:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.RightInf.Angle[0], Robot.RightInf.Angle[1], Robot.RightInf.Angle[2], Robot.RightInf.Angle[3], Robot.RightInf.Angle[4], Robot.RightInf.Angle[5]);

	}
	if (ImGui::Button(u8"左臂抱闸点"))
	{
		float InitBuf[6] = { -90,0,165,-15,0,0 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf, 90, 0, false);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂初始点")) // 
	{
		float InitBuf2[6] = { -1.447419, -0.812998, 2.088395, 1.340412, -1.570605, -0.37296 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf2);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"右臂初始点")) // 
	{
		float InitBuf[6] = { 0.000000, -2.268928, 1.396263, -0.000000, -0.785398, 1.308997 };//0,-129,80,0,-45,74.5
		Robot.Set(RightArm, PTP_Angle, InitBuf);
	}


	if (ImGui::Button(u8"相机识别")) // 
	{
		//system("D:\\bgr\\Lidar2\\x64\\Lidar2.exe");

		system("D:\\WorkSpace\\program\\PCL_ZED\\build\\Release\\ZED_with_PCL.exe");//上方主线
		printf("完成");
	}
	ImGui::SameLine();

	if (ImGui::Button(u8"雷达识别")) // 
	{
		system("D:\\bgr\\Lidar2\\x64\\Debug\\Lidar2.exe");

		//system("D:\\WorkSpace\\program\\PCL_ZED\\build\\Release\\ZED_with_PCL.exe");//上方主线
		printf("完成");
	}
	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"抓线");
	ImGui::SameLine();
	if (ImGui::Button(u8"选择抓线点")) // 
	{
		system("D:\\bgr\\Lidar3\\x64\\Debug\\Lidar2.exe");
		//ChuanxianRotationAndUp(-0.45);
		printf("完成");
	}
	if (ImGui::Button(u8"去抓线")) // 
	{

		//ChuanxianRotationAndUp(-0.45);
		printf("完成");
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"穿线");
	ImGui::SameLine();
	if (ImGui::Button(u8"穿线-0.45")) // 
	{

		ChuanxianRotationAndUp(-0.45);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.1")) // 
	{
		ChuanxianRotationAndUp(-0.1);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.0")) // 
	{
		ChuanxianRotationAndUp(0.0);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.1")) // 
	{
		ChuanxianRotationAndUp(0.1);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.15")) // 
	{
		ChuanxianRotationAndUp(0.15);
		//ZiDongJieHuoChuaaXian1();
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.2")) // 
	{
		ChuanxianRotationAndUp(0.2);
		//ZiDongJieHuoChuaaXian1();
		printf("完成");
	}
	if (ImGui::Button(u8"右臂后撤")) // 
	{
		ToolConnectCom.command(toolID, 1, 0, 0);
		Sleep(5000);
		ToolConnectCom.command(2, 1, 0, 0);
		Sleep(2000);
		float input[6] = { 0, 0, -0.2 * 1000, 0, 0, 0 };
		Robot.Set(RightArm, PTPRel_TCP, input);
		//input[2] = +0.1 * 1000;
		//input[1] = +0.1 * 1000;
		//rightArm.MoveS(input);
		Sleep(500);

		//float input2[6] = { 1.569824, -0.526339, -2.201829, -0.027255, 1.571391, 0.039929 };
		//rightArm.MoveJ(input2);
		//缺少回初始点
		printf("完成");
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"短杆接火装置");
	//ImGui::Text(u8"接火装置");//双臂自动接火作业
	ImGui::SameLine();
	if (ImGui::Button(u8"旋转上举-0.3##1")) // 
	{
		//RotationAndUpTest(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
		MoveWork("旋转上举-0.3");
		//RotationAndUp(ShortArm_X_Offset, -0.3, ShortArm_Z_Offset);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.2##1")) // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.2, ShortArm_Z_Offset);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.1##1")) // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.0##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.0, ShortArm_Z_Offset);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.1##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.1, ShortArm_Z_Offset);
		printf("完成");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.2##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.2, ShortArm_Z_Offset);
		printf("完成");
	}


	if (ImGui::Button(u8"放下复位")) // 
	{
		ZIDongJieHuoFuWei(toolID);//7号工具自动接火松开复位
		printf("完成");

	}

	ImGui::SameLine();
	if (ImGui::Button(u8"左臂接火装置初始位置")) // 
	{
		float input[6] = { 0, 0, -0.2 * 1000, 0, 0, 0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);
		Sleep(1000);
		float inputJoint[6] = { -1.57079, 1.04719, 2.61799, 0, -1.57079, 2.914699 };
		//float inputJoint[6] = { -1.57079, 1.04719, 2.61799, 0, -1.57079,0 };
		Robot.Set(RightArm, PTP_Angle, inputJoint);
		printf("完成");
	}
	ImGui::Text(u8"引线抓手");
	ImGui::SameLine();
	if (ImGui::Button(u8"开"))                            // 
	{
		ToolConnectCom.command(2, 1, 0, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"关"))                            // 
	{
		ToolConnectCom.command(2, 2, 0, 0);
	}
	ImGui::Text(u8"自动接火装置");
	ImGui::SameLine();
	if (ImGui::Button(u8"支线夹紧"))                            // 
	{
		ToolConnectCom.command(toolID, 1, 0, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"支线解锁"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 1, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"锁紧线夹"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 0, 1);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"停止锁紧"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 0, 2);
	}
	if (ImGui::Button(u8"长杆上举"))                            // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
	}

}

void GuZhangZhiShiQiUI()
{
	if (ImGui::Button(u8"左臂打开抱闸"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, OpenBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂关闭抱闸"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, CloseBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂重启脚本"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, RestartScrip);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂映射##1"))                            // 
	{
		Robot.LeftInf.Sync = !Robot.LeftInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.LeftInf.Sync == 1)
	{
		ImGui::Text(u8"左映射");
	}
	else
	{
		ImGui::Text(u8"左不映射");
	}

	if (ImGui::Button(u8"右臂映射##1"))                            // 
	{
		Robot.RightInf.Sync = !Robot.RightInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.RightInf.Sync == 1)
	{
		ImGui::Text(u8"右映射");
	}
	else
	{
		ImGui::Text(u8"右不映射");
	}

	if (ImGui::Button(u8"			停止机器人记录点		")) // 
	{
		Robot.Set(LeftArm, StopRobot);
		printf("左臂角度:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.LeftInf.Angle[0], Robot.LeftInf.Angle[1], Robot.LeftInf.Angle[2], Robot.LeftInf.Angle[3], Robot.LeftInf.Angle[4], Robot.LeftInf.Angle[5]);
		printf("右臂角度:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.RightInf.Angle[0], Robot.RightInf.Angle[1], Robot.RightInf.Angle[2], Robot.RightInf.Angle[3], Robot.RightInf.Angle[4], Robot.RightInf.Angle[5]);

	}
	if (ImGui::Button(u8"左臂抱闸点"))
	{
		float InitBuf[6] = { -90,0,165,-15,0,0 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf, 90, 0, false);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"左臂初始点")) // 
	{
		float InitBuf2[6] = { -1.447419, -0.812998, 2.088395, 1.340412, -1.570605, -0.37296 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf2);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"右臂初始点")) // 
	{
		float InitBuf[6] = { 0.000000, -2.268928, 1.396263, -0.000000, -0.785398, 1.308997 };//0,-129,80,0,-45,74.5
		Robot.Set(RightArm, PTP_Angle, InitBuf);
	}
	
	if (ImGui::Button(u8"故障指示器上举测试")) // 
	{
		GuZhangZhiShiQiRotationAndUpTest(0.0, -0.3, 0.0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"故障指示器上举1")) // 
	{

		GuZhangZhiShiQiRotationAndUp(0.0, -0.3, 0.0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"故障指示器上举2")) // 
	{

		GuZhangZhiShiQiRotationAndUp(0.01, -0.3, 0.05);
	}
	if (ImGui::Button(u8"故障指示器上举")) // 
	{

		float input[6] = { 0.0, 0.0, 50 ,0,0,0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);//单位毫米
	}
	if (ImGui::Button(u8"故障指示器下降")) // 
	{

		//GuZhangZhiShiQiRotationAndUp(0.0, -0.3, 0.0);
		float input[6] = { 0.0, 0.0, -50 ,0,0,0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);//单位毫米
	}

}