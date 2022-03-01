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

static float  LeftValue = 1.0f;		//���ʾ�̱���
static float  RightValue = 5.0f;		//�ұ�ʾ�̱���
bool Teach_pendant = false;
int toolID = 7;


//����
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

	window = glfwCreateWindow(1600, 900, u8" ����������ҵ������ң��������ϵͳ", NULL, NULL);
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
	//����ͼ�����
	int frame_padding = 3;                             // -1 == uses default padding (style.FramePadding)
	ImVec2 size = ImVec2(220.0f, 140.0f);                     // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint

	//����ͼ�����
	ImVec2 size2 = ImVec2(140.0f, 100.0f);                     // Size of the image we want to make visible
	//�򿪹ر�ͼ�����
	ImVec2 size3 = ImVec2(20.0f, 20.0f);                     // Size of the image we want to make visible
	//����ͼ�����
	ImVec2 size4 = ImVec2(112.0f, 140.0f);                     // Size of the image we want to make visible

	//1/1�����ڲ�ͼƬ
	ImVec2 size5 = ImVec2(480.0f, 40.0f);                     // Size of the image we want to make visible
	//1/2�����ڲ�ͼƬ
	ImVec2 size6 = ImVec2(232.0f, 40.0f);                     // Size of the image we want to make visible
	//1/4�����ڲ�ͼƬ
	ImVec2 size7 = ImVec2(108.0f, 40.0f);                     // Size of the image we want to make visible

	textureInit();

	//����logo��ʾ
	ImGui::Image((GLuint*)ImageID, ImVec2(500, 150));

	//����ģ��
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"����");

	ImGui::Checkbox(u8"ʾ������Ϣ", &Teach_pendant);

	//ImGui::SameLine();
	if (!Robot.LeftInf.Connect)
	{
		//ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"���δ����");

	}
	if (!Robot.LeftInf.Brake)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"���δ������բ");

	}
	if (!Robot.LeftInf.Script)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"��۽ű�����");

	}

	if (!Robot.RightInf.Connect)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), u8"�ұ�δ����");

	}

	TeachUI();

	ImGui::BeginChild(u8"����", ImVec2(500, 750), true);
	{
		if (ImGui::BeginTabBar("tools", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(u8"���߿���"))
			{
				//��ͬ����־λ
				//LeftTeleoperationFlag = false;
				//RightTeleoperationFlag = false;
				//������翪��
				static bool internet_left_tag = false;
				ImTextureID internet_left_tag_id;
				if (internet_left_tag == false)
					internet_left_tag_id = logo07_Id;
				else
					internet_left_tag_id = logo08_Id;
				if (ImGui::ImageButton(internet_left_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					internet_left_tag = !internet_left_tag;
					if (internet_left_tag == false)
					{
						Robot.CloseSocket();//�Ͽ�����
					}
					else if (internet_left_tag == true)
					{
						Robot.CreatSocket("192.168.1.100", 8010);//��������
					}
				}
				ImGui::SameLine();

				//�ұ����翪��
				static bool internet_right_tag = false;
				ImTextureID internet_right_tag_id;
				if (internet_right_tag == false)
					internet_right_tag_id = logo09_Id;
				else
					internet_right_tag_id = logo10_Id;
				if (ImGui::ImageButton(internet_right_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					internet_right_tag = !internet_right_tag;
					if (internet_right_tag == false)
					{
						Robot.CloseCom();//�Ͽ�����
					}
					else if (internet_right_tag == true)
					{
						Robot.OpenCom("COM1", 115200, 0, 8, 1);//��������
					}
				}
				ImGui::SameLine();

				//����ץ�ֿ���
				static bool line_grip_tag = false;
				ImTextureID line_grip_tag_id;
				if (line_grip_tag == false)
					line_grip_tag_id = logo11_Id;
				else
					line_grip_tag_id = logo12_Id;
				if (ImGui::ImageButton(line_grip_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
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

				//������ץ�ֿ���
				static bool lightning_protection_tag = false;
				ImTextureID lightning_protection_tag_id;
				if (lightning_protection_tag == false)
					lightning_protection_tag_id = logo13_Id;
				else
					lightning_protection_tag_id = logo14_Id;
				if (ImGui::ImageButton(lightning_protection_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					lightning_protection_tag = !lightning_protection_tag;
					if (lightning_protection_tag == false)
					{
						ToolConnectCom.command(1, 1, 0, 0);//������ץ�ֿ�

					}
					else if (lightning_protection_tag == true)
					{
						ToolConnectCom.command(1, 2, 0, 0);//������ץ�ֹ�
					}
				}
				ImGui::SameLine();

				//�綯������ת����
				static float  vol = 20.0f;		//�綯����ת��
				static bool electric_wrench_forward_tag = false;
				ImTextureID electric_wrench_forward_tag_id;
				if (electric_wrench_forward_tag == false)
					electric_wrench_forward_tag_id = logo15_Id;
				else
					electric_wrench_forward_tag_id = logo16_Id;
				if (ImGui::ImageButton(electric_wrench_forward_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					electric_wrench_forward_tag = !electric_wrench_forward_tag;
					if (electric_wrench_forward_tag == false)
					{
						ToolConnectCom.command(3, 0, 0, 0);//ͣ
					}
					else if (electric_wrench_forward_tag == true)
					{
						ToolConnectCom.command(3, 1, floor(vol), 0);//��ת
					}
				}
				ImGui::SameLine();

				//�綯���ַ�ת����
				static bool electric_wrench_reverse_tag = false;
				ImTextureID electric_wrench_reverse_tag_id;
				if (electric_wrench_reverse_tag == false)
					electric_wrench_reverse_tag_id = logo17_Id;
				else
					electric_wrench_reverse_tag_id = logo18_Id;
				if (ImGui::ImageButton(electric_wrench_reverse_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					electric_wrench_reverse_tag = !electric_wrench_reverse_tag;
					if (electric_wrench_reverse_tag == false)
					{
						ToolConnectCom.command(3, 0, 0, 0);//ͣ
					}
					else if (electric_wrench_reverse_tag == true)
					{
						ToolConnectCom.command(3, 2, floor(vol), 0);//��ת
					}
				}

				//�綯Һѹǯ����
				static bool electric_hydraulic_clamp_tag = false;
				ImTextureID electric_hydraulic_clamp_tag_id;
				if (electric_hydraulic_clamp_tag == false)
					electric_hydraulic_clamp_tag_id = logo19_Id;
				else
					electric_hydraulic_clamp_tag_id = logo20_Id;
				if (ImGui::ImageButton(electric_hydraulic_clamp_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
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

				//�Զ��ӻ�װ��1֧�߼н�
				static bool automatic_ignition_device_clamping_1_tag = false;
				ImTextureID automatic_ignition_device_clamping_1_tag_id;
				if (automatic_ignition_device_clamping_1_tag == false)
					automatic_ignition_device_clamping_1_tag_id = logo21_Id;
				else
					automatic_ignition_device_clamping_1_tag_id = logo22_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_1_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_clamping_1_tag = !automatic_ignition_device_clamping_1_tag;
					if (automatic_ignition_device_clamping_1_tag == false)
					{
						ToolConnectCom.command(5, 0, 1, 0);//����
					}
					else if (automatic_ignition_device_clamping_1_tag == true)
					{
						ToolConnectCom.command(5, 1, 0, 0);//�н�
					}
				}
				ImGui::SameLine();

				//�Զ��ӻ�װ��1š���߼�
				static bool automatic_ignition_device_tighten_1_tag = false;
				ImTextureID automatic_ignition_device_tighten_1_tag_id;
				if (automatic_ignition_device_tighten_1_tag == false)
					automatic_ignition_device_tighten_1_tag_id = logo23_Id;
				else
					automatic_ignition_device_tighten_1_tag_id = logo24_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_1_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_tighten_1_tag = !automatic_ignition_device_tighten_1_tag;
					if (automatic_ignition_device_tighten_1_tag == false)
					{
						ToolConnectCom.command(5, 0, 0, 2);//ֹͣš��
					}
					else if (automatic_ignition_device_tighten_1_tag == true)
					{
						ToolConnectCom.command(5, 0, 0, 1);//š���߼�
					}
				}

				//�Զ��ӻ�װ��2֧�߼н�
				static bool automatic_ignition_device_clamping_2_tag = false;
				ImTextureID automatic_ignition_device_clamping_2_tag_id;
				if (automatic_ignition_device_clamping_2_tag == false)
					automatic_ignition_device_clamping_2_tag_id = logo25_Id;
				else
					automatic_ignition_device_clamping_2_tag_id = logo26_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_2_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_clamping_2_tag = !automatic_ignition_device_clamping_2_tag;
					if (automatic_ignition_device_clamping_2_tag == false)
					{
						ToolConnectCom.command(7, 0, 1, 0);//����
					}
					else if (automatic_ignition_device_clamping_2_tag == true)
					{
						ToolConnectCom.command(7, 1, 0, 0);//�н�
					}
				}
				ImGui::SameLine();

				//�Զ��ӻ�װ��2š���߼�
				static bool automatic_ignition_device_tighten_2_tag = false;
				ImTextureID automatic_ignition_device_tighten_2_tag_id;
				if (automatic_ignition_device_tighten_2_tag == false)
					automatic_ignition_device_tighten_2_tag_id = logo27_Id;
				else
					automatic_ignition_device_tighten_2_tag_id = logo28_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_2_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_tighten_2_tag = !automatic_ignition_device_tighten_2_tag;
					if (automatic_ignition_device_tighten_2_tag == false)
					{
						ToolConnectCom.command(7, 0, 0, 2);//ֹͣš��
					}
					else if (automatic_ignition_device_tighten_2_tag == true)
					{
						ToolConnectCom.command(7, 0, 0, 1);//š���߼�
					}
				}
				ImGui::SameLine();

				//�Զ��ӻ�װ��3֧�߼н�
				static bool automatic_ignition_device_clamping_3_tag = false;
				ImTextureID automatic_ignition_device_clamping_3_tag_id;
				if (automatic_ignition_device_clamping_3_tag == false)
					automatic_ignition_device_clamping_3_tag_id = logo29_Id;
				else
					automatic_ignition_device_clamping_3_tag_id = logo30_Id;
				if (ImGui::ImageButton(automatic_ignition_device_clamping_3_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_clamping_3_tag = !automatic_ignition_device_clamping_3_tag;
					if (automatic_ignition_device_clamping_3_tag == false)
					{
						ToolConnectCom.command(9, 0, 1, 0);//����
					}
					else if (automatic_ignition_device_clamping_3_tag == true)
					{
						ToolConnectCom.command(9, 1, 0, 0);//�н�
					}
				}

				//�Զ��ӻ�װ��3š���߼�
				static bool automatic_ignition_device_tighten_3_tag = false;
				ImTextureID automatic_ignition_device_tighten_3_tag_id;
				if (automatic_ignition_device_tighten_3_tag == false)
					automatic_ignition_device_tighten_3_tag_id = logo31_Id;
				else
					automatic_ignition_device_tighten_3_tag_id = logo32_Id;
				if (ImGui::ImageButton(automatic_ignition_device_tighten_3_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					automatic_ignition_device_tighten_3_tag = !automatic_ignition_device_tighten_3_tag;
					if (automatic_ignition_device_tighten_3_tag == false)
					{
						ToolConnectCom.command(9, 0, 0, 2);//ֹͣš��
					}
					else if (automatic_ignition_device_tighten_3_tag == true)
					{
						ToolConnectCom.command(9, 0, 0, 1);//š���߼�
					}
				}
				ImGui::SameLine();

				//���ӳ�俪��
				static bool left_arm_mapping_tag = false;
				ImTextureID left_arm_mapping_tag_id;
				if (left_arm_mapping_tag == false)
					left_arm_mapping_tag_id = logo33_Id;
				else
					left_arm_mapping_tag_id = logo34_Id;
				if (ImGui::ImageButton(left_arm_mapping_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					left_arm_mapping_tag = !left_arm_mapping_tag;
					if (left_arm_mapping_tag == false)
					{
						Robot.LeftInf.Sync = 0;//���ӳ���
					}
					else if (left_arm_mapping_tag == true)
					{
						Robot.LeftInf.Sync = 1;//���ӳ�俪
					}
				}
				ImGui::SameLine();

				//�ұ�ӳ�俪��
				static bool right_arm_mapping_tag = false;
				ImTextureID right_arm_mapping_tag_id;
				if (right_arm_mapping_tag == false)
					right_arm_mapping_tag_id = logo35_Id;
				else
					right_arm_mapping_tag_id = logo36_Id;
				if (ImGui::ImageButton(right_arm_mapping_tag_id, size2, uv0, uv1, frame_padding, bg_col, tint_col))
				{
					printf("�����ť��\n");
					right_arm_mapping_tag = !right_arm_mapping_tag;
					if (right_arm_mapping_tag == false)
					{
						Robot.RightInf.Sync = 0;//�ұ�ӳ���
					}
					else if (right_arm_mapping_tag == true)
					{
						Robot.RightInf.Sync = 1;//�ұ�ӳ�俪
					}
				}

				ImGui::SliderFloat("  ", &vol, 0.0f, 100.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"Ť��");

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"���ң����"))
			{
				{
					float Angle_Dis[6] = { 0,0,0,0,0,0 };
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//��ǰ
					if (ImGui::ImageButton(direction_forward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[1] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					if (ImGui::ImageButton(direction_up_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[2] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					/*************��һ��****************/
					//����
					if (ImGui::ImageButton(direction_left_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[0] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					if (ImGui::ImageButton(direction_right_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[0] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					/*************��һ��****************/
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//���
					if (ImGui::ImageButton(direction_backward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[1] = LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					if (ImGui::ImageButton(direction_down_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						Angle_Dis[2] = -LeftValue;
						Robot.Set(LeftArm, PTPRel_TCP, Angle_Dis);
					}
					//ImGui::SameLine();
				}
				ImGui::SliderFloat("  ", &LeftValue, 0.1f, 5.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��ʾ�̱���");
				//ImGui::EndChild();				
				ImGui::EndTabItem();

			}

			if (ImGui::BeginTabItem(u8"�ұ�ң����"))
			{
				{
					vector<float> AngleDif(6, 0);
						//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//��ǰ
					if (ImGui::ImageButton(direction_forward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[1] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					ImGui::PushButtonRepeat(true);
					if (ImGui::ImageButton(direction_up_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
						//if (ImGui::Button(u8"�ұ۷�������"))
					{
						AngleDif[2] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::PopButtonRepeat();
					/*************��һ��****************/
					//����
					if (ImGui::ImageButton(direction_left_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[0] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					if (ImGui::ImageButton(direction_right_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[0] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					/*************��һ��****************/
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//���
					if (ImGui::ImageButton(direction_backward_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[1] = RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
					ImGui::SameLine();
					//ռλ
					ImGui::Text("			");
					ImGui::SameLine();
					//����
					if (ImGui::ImageButton(direction_down_id, size4, uv0, uv1, frame_padding, bg_col, tint_col))
					{
						AngleDif[2] = -RightValue;
						Robot.Set(RightArm, PTPRel_TCP, &AngleDif[0]);
					}
				}

				/*ImGui::SameLine();*/
				ImGui::SliderFloat("  ", &RightValue, 0.1f, 10.0f); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��ʾ�̱���");
				ImGui::EndTabItem();
			}
		}
	}
	ImGui::EndChild();

	//��һ��
	ImGui::NextColumn();

	//������ҵģ��
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"������ҵ");
	static bool work_initialize_tag = true;        //��ʾ������ҵʱΪtrue
	static bool function01_tag = false;            //������ҵ1�˱�־Ϊtrue
	static bool function02_tag = false;            //������ҵ2�˱�־Ϊtrue
	static bool function03_tag = false;            //������ҵ3�˱�־Ϊtrue
	static bool function04_tag = false;            //������ҵ4�˱�־Ϊtrue
	static bool function05_tag = false;            //������ҵ5�˱�־Ϊtrue
	static bool function06_tag = false;            //������ҵ6�˱�־Ϊtrue
	static bool function07_tag = false;            //������ҵ7�˱�־Ϊtrue
	static bool function08_tag = false;            //������ҵ8�˱�־Ϊtrue
	static bool function09_tag = false;            //������ҵ9�˱�־Ϊtrue
	static bool enter_main_tag = true;			   //Ϊ����������bug��Ϊtrueʱ��ʾ��ҵ������
	ImGui::BeginChild(u8"��ҵ", ImVec2(500, 900), true);
	{
		enter_main_tag = true;
		//�����֧����·������ҵ
		if (function01_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��֧����·����   ������");
			JieHuoUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function01_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//�����֧����·������ҵ
		if (function02_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��֧����·����   ����������");
			//JianXianUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function02_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//���������������ҵ
		if (function03_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"����������");
			//BiLeiQiUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function03_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//����ӵ���ʽ�۶�����������ҵ
		if (function04_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"�ӵ���ʽ�۶���������");
			//JieHuoUI();

			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function04_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//����ϵ���ʽ�۶�����������ҵ
		if (function05_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"�ϵ���ʽ�۶���������");
			//JianXianUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function05_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//�����������ʽ�۶�����ҵ
		if (function06_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��������ʽ�۶���");
			//BaoXianQiUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function06_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//����װ����ָʾ����ҵ
		if (function07_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"װ����ָʾ��");
			//GuZhangZhiShiQiUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function07_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//��������ָʾ����ҵ
		if (function08_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"�����ָʾ��");
			GuZhangZhiShiQiUI();

			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function08_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}
		//�����֦��ҵ
		if (function09_tag == true)
		{
			ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"��֦");
			//JianXianUI();
			if (ImGui::Button(u8"���ؾ�����ҵ"))
			{
				function09_tag = false;
				work_initialize_tag = true;
				enter_main_tag = false;
			}
		}

		//��ʾ������ҵ��ҳ
		if (work_initialize_tag == true && enter_main_tag == true)
		{
			if (ImGui::ImageButton(function_logo_01_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function01_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_02_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function02_tag = true;
			}

			if (ImGui::ImageButton(function_logo_03_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function03_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_04_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function04_tag = true;
			}

			if (ImGui::ImageButton(function_logo_05_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function05_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_06_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function06_tag = true;
			}

			if (ImGui::ImageButton(function_logo_07_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function07_tag = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton(function_logo_08_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function08_tag = true;
			}

			if (ImGui::ImageButton(function_logo_09_Id, size, uv0, uv1, frame_padding, bg_col, tint_col))
			{
				printf("�����ť��\n");
				work_initialize_tag = false;
				function09_tag = true;
			}
		}

	}
	ImGui::EndChild();

	//��һ��
	ImGui::NextColumn();

	//�������
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"���");
	ImGui::BeginChild(u8"�������", ImVec2(500, 370), true);
	{
		if (ImGui::BeginTabBar("video", ImGuiTabBarFlags_None))
		{
			//�������ʾ
			if (ImGui::BeginTabItem(u8"�����"))
			{
				static bool rtspFlag1 = false;

				if (ImGui::Button(u8"��"))
				{
					FlagWetCamera = vcap.open(videoStreamAddress);
					if (!rtspFlag1)
					{
						rtspFlag1 = true;
						glGenTextures(1, &CameraID);
					}
				}

				ImGui::SameLine();
				if (ImGui::Button(u8"�ر�"))
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
			//���������ʾ
			if (ImGui::BeginTabItem(u8"�������"))
			{
				
			}
			//�����������ʾ
			if (ImGui::BeginTabItem(u8"���������"))
			{

			}

		}

	}
	ImGui::EndChild();


	//Vrepͼ��
	ImGui::TextColored(ImVec4(0.0f, 0.0f, 0.0f, 1.0f), u8"vrepͼ��");
	ImGui::BeginChild(u8"Vrepͼ��", ImVec2(500, 500), true);
	{
		//vrepͼ����ʾ
		//ImGui::Image((GLuint*)VrepLogoID, ImVec2(500, 30));
		static bool vrepFlag = false;
		if (ImGui::Button(u8"��"))
		{
			Robot.FlagVrepImg = true;
			if (!vrepFlag)
			{
				vrepFlag = true;
				glGenTextures(0, &VrepID);
			}
		}

		ImGui::SameLine();
		if (ImGui::Button(u8"�ر�"))
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

	// //���ô��ڵ�paddingΪ0��ͼƬ�ؼ���������
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	//���ô��ڵı߿��СΪ0
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);

	ImGui::StyleColorsBlue();


	bool start_tag = true;
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::Begin(u8"��������Ϣ", &start_tag, 1 << 10);
	ImGui::Columns(3, 0, false);
	ImGui::Separator();

	//����
	//��ʾ�½���
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
		
		//��۽Ƕ���ʾ
		{
			ImGui::LabelText(u8"���ɱ۽Ƕ�", u8"     �ٿ�");
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
		//�ұ۽Ƕ���ʾ
		{
			ImGui::LabelText(u8"Һѹ�۽Ƕ�", u8"     �ٿ�");
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

	if (ImGui::Button(u8"��۴򿪱�բ"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, OpenBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��۹رձ�բ"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, CloseBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��������ű�"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, RestartScrip);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"���ӳ��##1"))                            // 
	{
		Robot.LeftInf.Sync = !Robot.LeftInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.LeftInf.Sync == 1)
	{
		ImGui::Text(u8"��ӳ��");
	}
	else
	{
		ImGui::Text(u8"��ӳ��");
	}

	if (ImGui::Button(u8"�ұ�ӳ��##1"))                            // 
	{
		Robot.RightInf.Sync = !Robot.RightInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.RightInf.Sync == 1)
	{
		ImGui::Text(u8"��ӳ��");
	}
	else
	{
		ImGui::Text(u8"�Ҳ�ӳ��");
	}

	if (ImGui::Button(u8"			ֹͣ�����˼�¼��		")) // 
	{
		Robot.Set(LeftArm, StopRobot);
		printf("��۽Ƕ�:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.LeftInf.Angle[0], Robot.LeftInf.Angle[1], Robot.LeftInf.Angle[2], Robot.LeftInf.Angle[3], Robot.LeftInf.Angle[4], Robot.LeftInf.Angle[5]);
		printf("�ұ۽Ƕ�:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.RightInf.Angle[0], Robot.RightInf.Angle[1], Robot.RightInf.Angle[2], Robot.RightInf.Angle[3], Robot.RightInf.Angle[4], Robot.RightInf.Angle[5]);

	}
	if (ImGui::Button(u8"��۱�բ��"))
	{
		float InitBuf[6] = { -90,0,165,-15,0,0 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf, 90, 0, false);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��۳�ʼ��")) // 
	{
		float InitBuf2[6] = { -1.447419, -0.812998, 2.088395, 1.340412, -1.570605, -0.37296 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf2);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"�ұ۳�ʼ��")) // 
	{
		float InitBuf[6] = { 0.000000, -2.268928, 1.396263, -0.000000, -0.785398, 1.308997 };//0,-129,80,0,-45,74.5
		Robot.Set(RightArm, PTP_Angle, InitBuf);
	}


	if (ImGui::Button(u8"���ʶ��")) // 
	{
		//system("D:\\bgr\\Lidar2\\x64\\Lidar2.exe");

		system("D:\\WorkSpace\\program\\PCL_ZED\\build\\Release\\ZED_with_PCL.exe");//�Ϸ�����
		printf("���");
	}
	ImGui::SameLine();

	if (ImGui::Button(u8"�״�ʶ��")) // 
	{
		system("D:\\bgr\\Lidar2\\x64\\Debug\\Lidar2.exe");

		//system("D:\\WorkSpace\\program\\PCL_ZED\\build\\Release\\ZED_with_PCL.exe");//�Ϸ�����
		printf("���");
	}
	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"ץ��");
	ImGui::SameLine();
	if (ImGui::Button(u8"ѡ��ץ�ߵ�")) // 
	{
		system("D:\\bgr\\Lidar3\\x64\\Debug\\Lidar2.exe");
		//ChuanxianRotationAndUp(-0.45);
		printf("���");
	}
	if (ImGui::Button(u8"ȥץ��")) // 
	{

		//ChuanxianRotationAndUp(-0.45);
		printf("���");
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"����");
	ImGui::SameLine();
	if (ImGui::Button(u8"����-0.45")) // 
	{

		ChuanxianRotationAndUp(-0.45);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.1")) // 
	{
		ChuanxianRotationAndUp(-0.1);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.0")) // 
	{
		ChuanxianRotationAndUp(0.0);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.1")) // 
	{
		ChuanxianRotationAndUp(0.1);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.15")) // 
	{
		ChuanxianRotationAndUp(0.15);
		//ZiDongJieHuoChuaaXian1();
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.2")) // 
	{
		ChuanxianRotationAndUp(0.2);
		//ZiDongJieHuoChuaaXian1();
		printf("���");
	}
	if (ImGui::Button(u8"�ұۺ�")) // 
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
		//ȱ�ٻس�ʼ��
		printf("���");
	}

	ImGui::TextColored(ImVec4(1, 0, 0, 1), u8"�̸˽ӻ�װ��");
	//ImGui::Text(u8"�ӻ�װ��");//˫���Զ��ӻ���ҵ
	ImGui::SameLine();
	if (ImGui::Button(u8"��ת�Ͼ�-0.3##1")) // 
	{
		//RotationAndUpTest(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
		MoveWork("��ת�Ͼ�-0.3");
		//RotationAndUp(ShortArm_X_Offset, -0.3, ShortArm_Z_Offset);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.2##1")) // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.2, ShortArm_Z_Offset);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"-0.1##1")) // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.0##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.0, ShortArm_Z_Offset);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.1##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.1, ShortArm_Z_Offset);
		printf("���");
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"0.2##1")) // 
	{

		RotationAndUp(ShortArm_X_Offset, 0.2, ShortArm_Z_Offset);
		printf("���");
	}


	if (ImGui::Button(u8"���¸�λ")) // 
	{
		ZIDongJieHuoFuWei(toolID);//7�Ź����Զ��ӻ��ɿ���λ
		printf("���");

	}

	ImGui::SameLine();
	if (ImGui::Button(u8"��۽ӻ�װ�ó�ʼλ��")) // 
	{
		float input[6] = { 0, 0, -0.2 * 1000, 0, 0, 0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);
		Sleep(1000);
		float inputJoint[6] = { -1.57079, 1.04719, 2.61799, 0, -1.57079, 2.914699 };
		//float inputJoint[6] = { -1.57079, 1.04719, 2.61799, 0, -1.57079,0 };
		Robot.Set(RightArm, PTP_Angle, inputJoint);
		printf("���");
	}
	ImGui::Text(u8"����ץ��");
	ImGui::SameLine();
	if (ImGui::Button(u8"��"))                            // 
	{
		ToolConnectCom.command(2, 1, 0, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��"))                            // 
	{
		ToolConnectCom.command(2, 2, 0, 0);
	}
	ImGui::Text(u8"�Զ��ӻ�װ��");
	ImGui::SameLine();
	if (ImGui::Button(u8"֧�߼н�"))                            // 
	{
		ToolConnectCom.command(toolID, 1, 0, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"֧�߽���"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 1, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"�����߼�"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 0, 1);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"ֹͣ����"))                            // 
	{
		ToolConnectCom.command(toolID, 0, 0, 2);
	}
	if (ImGui::Button(u8"�����Ͼ�"))                            // 
	{
		RotationAndUp(ShortArm_X_Offset, -0.1, ShortArm_Z_Offset);
	}

}

void GuZhangZhiShiQiUI()
{
	if (ImGui::Button(u8"��۴򿪱�բ"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, OpenBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��۹رձ�բ"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, CloseBrake);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��������ű�"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		Robot.Set(LeftArm, RestartScrip);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"���ӳ��##1"))                            // 
	{
		Robot.LeftInf.Sync = !Robot.LeftInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.LeftInf.Sync == 1)
	{
		ImGui::Text(u8"��ӳ��");
	}
	else
	{
		ImGui::Text(u8"��ӳ��");
	}

	if (ImGui::Button(u8"�ұ�ӳ��##1"))                            // 
	{
		Robot.RightInf.Sync = !Robot.RightInf.Sync;
	}
	ImGui::SameLine();
	if (Robot.RightInf.Sync == 1)
	{
		ImGui::Text(u8"��ӳ��");
	}
	else
	{
		ImGui::Text(u8"�Ҳ�ӳ��");
	}

	if (ImGui::Button(u8"			ֹͣ�����˼�¼��		")) // 
	{
		Robot.Set(LeftArm, StopRobot);
		printf("��۽Ƕ�:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.LeftInf.Angle[0], Robot.LeftInf.Angle[1], Robot.LeftInf.Angle[2], Robot.LeftInf.Angle[3], Robot.LeftInf.Angle[4], Robot.LeftInf.Angle[5]);
		printf("�ұ۽Ƕ�:%lf,%lf,%lf,%lf,%lf,%lf\n", Robot.RightInf.Angle[0], Robot.RightInf.Angle[1], Robot.RightInf.Angle[2], Robot.RightInf.Angle[3], Robot.RightInf.Angle[4], Robot.RightInf.Angle[5]);

	}
	if (ImGui::Button(u8"��۱�բ��"))
	{
		float InitBuf[6] = { -90,0,165,-15,0,0 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf, 90, 0, false);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"��۳�ʼ��")) // 
	{
		float InitBuf2[6] = { -1.447419, -0.812998, 2.088395, 1.340412, -1.570605, -0.37296 };
		Robot.Set(LeftArm, PTP_Angle, InitBuf2);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"�ұ۳�ʼ��")) // 
	{
		float InitBuf[6] = { 0.000000, -2.268928, 1.396263, -0.000000, -0.785398, 1.308997 };//0,-129,80,0,-45,74.5
		Robot.Set(RightArm, PTP_Angle, InitBuf);
	}
	
	if (ImGui::Button(u8"����ָʾ���Ͼٲ���")) // 
	{
		GuZhangZhiShiQiRotationAndUpTest(0.0, -0.3, 0.0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"����ָʾ���Ͼ�1")) // 
	{

		GuZhangZhiShiQiRotationAndUp(0.0, -0.3, 0.0);
	}
	ImGui::SameLine();
	if (ImGui::Button(u8"����ָʾ���Ͼ�2")) // 
	{

		GuZhangZhiShiQiRotationAndUp(0.01, -0.3, 0.05);
	}
	if (ImGui::Button(u8"����ָʾ���Ͼ�")) // 
	{

		float input[6] = { 0.0, 0.0, 50 ,0,0,0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);//��λ����
	}
	if (ImGui::Button(u8"����ָʾ���½�")) // 
	{

		//GuZhangZhiShiQiRotationAndUp(0.0, -0.3, 0.0);
		float input[6] = { 0.0, 0.0, -50 ,0,0,0 };
		Robot.Set(LeftArm, PTPRel_TCP, input);//��λ����
	}

}