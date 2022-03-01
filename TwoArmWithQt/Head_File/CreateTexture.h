#pragma once
#include "cvTest.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "ImageTexture.h"

extern ToolConnect ToolConnectCom;
extern RobotArm Robot;

//ͼƬid
GLuint IconID;
GLuint ImageID;
GLuint CameraID;
GLuint CameraID2;
GLuint CameraID3;
GLuint VrepID;
GLuint ToolImageID;
GLuint FunctionImageID;
GLuint CameraLogoID;
GLuint VrepLogoID;
GLuint CameraCloseImageID;
GLuint LeftControlImageID;
GLuint RightControlImageID;
GLuint Work_01_ID;//��֧����·�����ڲ���ʾͼƬ



//����ͼ������id
ImTextureID function_logo_01_Id;
ImTextureID function_logo_02_Id;
ImTextureID function_logo_03_Id;
ImTextureID function_logo_04_Id;
ImTextureID function_logo_05_Id;
ImTextureID function_logo_06_Id;
ImTextureID function_logo_07_Id;
ImTextureID function_logo_08_Id;
ImTextureID function_logo_09_Id;

//����ͼ������id
ImTextureID direction_forward_id;
ImTextureID direction_backward_id;
ImTextureID direction_left_id;
ImTextureID direction_right_id;
ImTextureID direction_up_id;
ImTextureID direction_down_id;

//����ͼ������id
ImTextureID logo07_Id;//�������
ImTextureID logo08_Id;
ImTextureID logo09_Id;//�ұ�����
ImTextureID logo10_Id;
ImTextureID logo11_Id;//����ץ��
ImTextureID logo12_Id;
ImTextureID logo13_Id;//������ץ��
ImTextureID logo14_Id;
ImTextureID logo15_Id;//�綯������ת
ImTextureID logo16_Id;
ImTextureID logo17_Id;//�綯���ַ�ת
ImTextureID logo18_Id;
ImTextureID logo19_Id;//�綯Һѹǯ
ImTextureID logo20_Id;
ImTextureID logo21_Id;//�Զ��ӻ�װ��1_֧�߼н�
ImTextureID logo22_Id;
ImTextureID logo23_Id;//�Զ��ӻ�װ��1_š���߼�
ImTextureID logo24_Id;
ImTextureID logo25_Id;//�Զ��ӻ�װ��2_֧�߼н�
ImTextureID logo26_Id;
ImTextureID logo27_Id;//�Զ��ӻ�װ��2_š���߼�
ImTextureID logo28_Id;
ImTextureID logo29_Id;//�Զ��ӻ�װ��3_֧�߼н�
ImTextureID logo30_Id;
ImTextureID logo31_Id;//�Զ��ӻ�װ��3_š���߼�
ImTextureID logo32_Id;
ImTextureID logo33_Id;//���ӳ��
ImTextureID logo34_Id;
ImTextureID logo35_Id;//�ұ�ӳ��
ImTextureID logo36_Id;

//�򿪹ر�ͼ������id
ImTextureID OpenImageID;
ImTextureID CloseImageID;

//��֧����·�����ڲ�ͼƬ����
ImTextureID function_01_01_id;//ң��������
ImTextureID function_01_02_id;//�Զ�����
ImTextureID function_01_03_id;//�ұۻ�����λ��
ImTextureID function_01_04_id;//ȡ����1����ץ��
ImTextureID function_01_05_id;//ȡ4��λ�ӻ�װ��
ImTextureID function_01_06_id;//���ץ��λ��
ImTextureID function_01_07_id;//���ץ�ֽӻ�λ��
ImTextureID function_01_08_id;//���ʶ��
ImTextureID function_01_09_id;//����0.5
ImTextureID function_01_10_id;//����0.3
ImTextureID function_01_11_id;//����0.2
ImTextureID function_01_12_id;//����0.1
ImTextureID function_01_13_id;//����0
ImTextureID function_01_14_id;//����-0.1
ImTextureID function_01_15_id;//��ۺ�
ImTextureID function_01_16_id;//����-0.1
ImTextureID function_01_17_id;//����
ImTextureID function_01_18_id;//����0.1
ImTextureID function_01_19_id;//����0.2
ImTextureID function_01_20_id;//���¸�λ
ImTextureID function_01_21_id;//�ұ۽ӻ�װ�ø�λ
ImTextureID function_01_22_id;//��4��λ�ӻ�װ��
ImTextureID function_01_23_id;//�Ź���1����ץ��
ImTextureID function_01_24_id;//���ؾ�����ҵ
ImTextureID function_01_25_id;//�ӻ����
ImTextureID function_01_26_id;//��۳�ʼλ��







//����ͼƬ����
void ImageCreateTexture(const String& m_fileName, GLuint& textureId)
{
	cv::Mat img = imread(m_fileName);
	if (img.empty())
	{
		printf("Can not load image %s\n", m_fileName);
	}
	else
	{
		//���ó���
		int width = img.cols;
		int height = img.rows;
		int channel = img.channels();
		//��ȡͼ��ָ��
		int pixellength = width * height * channel;
		GLubyte* pixels = new GLubyte[pixellength];
		memcpy(pixels, img.data, pixellength * sizeof(char));
		// ʹ��GLָ��������ͼ����ȡ��ͼID
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		//����һ��RGB  һ��BGR��opencv��mat�����ɫͨ����BGR�� ����������ɫƫ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		//����Ŵ���Сʹ�����Բ�ֵ
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		free(pixels);
	}
}
//�����������
void CameraCreateTexture(cv::VideoCapture vcap, GLuint id)
{
	static cv::Mat image;
	if (!vcap.read(image))
	{
		printf("Can not load Cameras\n");
	}
	else
	{
		//���ó���

		Size dsize = Size(1080, 720);
		//Mat img2 = Mat(dsize, CV_32S);
		resize(image, image, dsize);
		int width = image.cols;
		int height = image.rows;
		int channel = image.channels();
		//��ȡͼ��ָ��
		int pixellength = width * height * channel;
		cout << " ******************************************" << endl;
		cout << width << endl;
		static GLubyte* rtsppixels = new GLubyte[pixellength];
		memcpy(rtsppixels, image.data, pixellength * sizeof(char));

		glBindTexture(GL_TEXTURE_2D, id);
		//����һ��RGB  һ��BGR��opencv��mat�����ɫͨ����BGR�� ����������ɫƫ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, rtsppixels);
		//����Ŵ���Сʹ�����Բ�ֵ
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		//free(pixels);
		///image.release();
		//delete []pixels;
	}
}
//����vrep����
void VrepCreateTexture(simxUChar* vrep_image)
{
	static cv::Mat image;
	static cv::Mat image_fliped;//��Ҫ���¸����µ�Mat��������������û�гɹ���ת�����
	image = cv::Mat(Robot.resolution[0], Robot.resolution[1], CV_8UC3, vrep_image);//��������ͼ�������Ǵ�ֱ��ת��,����Ӧ������cvMat �� v-rep ��ֱ������ķ����෴,flipһ�¾�������

	cv::flip(image, image_fliped, 0);
	//��������ͼ������ʱrgbͨ���ֲ��ģ���cvMat Ĭ��bgr
	cv::cvtColor(image_fliped, image_fliped, cv::COLOR_RGB2BGR);
	if (image_fliped.empty())
	{
		printf("Can not load Cameras\n");
	}
	else
	{
		//���ó���
		int width = image_fliped.cols;
		int height = image_fliped.rows;
		int channel = image_fliped.channels();

		//��ȡͼ��ָ��
		int pixellength = width * height * channel;
		static GLubyte* pixels = new GLubyte[pixellength];
		memcpy(pixels, image_fliped.data, pixellength * sizeof(char));
		glBindTexture(GL_TEXTURE_2D, VrepID);

		//����һ��RGB  һ��BGR��opencv��mat�����ɫͨ����BGR�� ����������ɫƫ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		//����Ŵ���Сʹ�����Բ�ֵ
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*image.release();
		image_fliped.release();*/
		//free(pixels);
	}
}
//�����ʼ��
void textureInit() {
	static bool initFlag = false;
	if (!initFlag)
	{
		initFlag = true;

		ImageCreateTexture("icon/ͼƬ/���ҵ���.jpg", ImageID);
		ImageCreateTexture("icon/ͼƬ/���.jpg", CameraLogoID);
		ImageCreateTexture("icon/ͼƬ/��ҵ.jpg", FunctionImageID);
		ImageCreateTexture("icon/ͼƬ/vrepͼ��.jpg", VrepLogoID);
		ImageCreateTexture("icon/ͼƬ/����.jpg", ToolImageID);
		ImageCreateTexture("icon/ͼƬ/����ͷ�ر�.jpg", CameraCloseImageID);
		ImageCreateTexture("icon/ͼƬ/���ң����.jpg", LeftControlImageID);
		ImageCreateTexture("icon/ͼƬ/�ұ�ң����.jpg", RightControlImageID);
		ImageCreateTexture("icon/������ҵ/��֧����·����/��֧����·����.jpg", Work_01_ID);



		ImageTexture openLogo;
		openLogo.createTexture("icon/ͼƬ/��.jpg");
		OpenImageID = (GLuint *)openLogo.textureId;

		ImageTexture closeLogo;
		closeLogo.createTexture("icon/ͼƬ/�ر�.jpg");
		CloseImageID = (GLuint *)closeLogo.textureId;

		/********************************����ͼ��***********************************/
		ImageTexture function_logo_01;
		function_logo_01.createTexture("icon/����ͼ��/��֧����·����.jpg");
		function_logo_01_Id = (GLuint *)function_logo_01.textureId;

		ImageTexture function_logo_02;
		function_logo_02.createTexture("icon/����ͼ��/��֧����·����.jpg");
		function_logo_02_Id = (GLuint *)function_logo_02.textureId;

		ImageTexture function_logo_03;
		function_logo_03.createTexture("icon/����ͼ��/����������.jpg");
		function_logo_03_Id = (GLuint *)function_logo_03.textureId;

		ImageTexture function_logo_04;
		function_logo_04.createTexture("icon/����ͼ��/�ӵ���ʽ�۶���������.jpg");
		function_logo_04_Id = (GLuint *)function_logo_04.textureId;

		ImageTexture function_logo_05;
		function_logo_05.createTexture("icon/����ͼ��/�ϵ���ʽ�۶���������.jpg");
		function_logo_05_Id = (GLuint *)function_logo_05.textureId;

		ImageTexture function_logo_06;
		function_logo_06.createTexture("icon/����ͼ��/��������ʽ�۶���.jpg");
		function_logo_06_Id = (GLuint *)function_logo_06.textureId;

		ImageTexture function_logo_07;
		function_logo_07.createTexture("icon/����ͼ��/װ����ָʾ��.jpg");
		function_logo_07_Id = (GLuint *)function_logo_07.textureId;

		ImageTexture function_logo_08;
		function_logo_08.createTexture("icon/����ͼ��/�����ָʾ��.jpg");
		function_logo_08_Id = (GLuint *)function_logo_08.textureId;

		ImageTexture function_logo_09;
		function_logo_09.createTexture("icon/����ͼ��/��֦.jpg");
		function_logo_09_Id = (GLuint *)function_logo_09.textureId;


		/********************************����ͼ��***********************************/
		ImageTexture logo07;
		logo07.createTexture("icon/����ͼ��/��������.jpg");
		logo07_Id = (GLuint *)logo07.textureId;

		ImageTexture logo08;
		logo08.createTexture("icon/����ͼ��/������翪.jpg");
		logo08_Id = (GLuint *)logo08.textureId;

		ImageTexture logo09;
		logo09.createTexture("icon/����ͼ��/�ұ������.jpg");
		logo09_Id = (GLuint *)logo09.textureId;

		ImageTexture logo10;
		logo10.createTexture("icon/����ͼ��/�ұ����翪.jpg");
		logo10_Id = (GLuint *)logo10.textureId;

		ImageTexture logo11;
		logo11.createTexture("icon/����ͼ��/����ץ�ֹ�.jpg");
		logo11_Id = (GLuint *)logo11.textureId;

		ImageTexture logo12;
		logo12.createTexture("icon/����ͼ��/����ץ�ֿ�.jpg");
		logo12_Id = (GLuint *)logo12.textureId;

		ImageTexture logo13;
		logo13.createTexture("icon/����ͼ��/������ץ�ֹ�.jpg");
		logo13_Id = (GLuint *)logo13.textureId;

		ImageTexture logo14;
		logo14.createTexture("icon/����ͼ��/������ץ�ֿ�.jpg");
		logo14_Id = (GLuint *)logo14.textureId;

		ImageTexture logo15;
		logo15.createTexture("icon/����ͼ��/�綯������ת��.jpg");
		logo15_Id = (GLuint *)logo15.textureId;

		ImageTexture logo16;
		logo16.createTexture("icon/����ͼ��/�綯������ת��.jpg");
		logo16_Id = (GLuint *)logo16.textureId;

		ImageTexture logo17;
		logo17.createTexture("icon/����ͼ��/�綯���ַ�ת��.jpg");
		logo17_Id = (GLuint *)logo17.textureId;

		ImageTexture logo18;
		logo18.createTexture("icon/����ͼ��/�綯���ַ�ת��.jpg");
		logo18_Id = (GLuint *)logo18.textureId;

		ImageTexture logo19;
		logo19.createTexture("icon/����ͼ��/�綯Һѹǯ��.jpg");
		logo19_Id = (GLuint *)logo19.textureId;

		ImageTexture logo20;
		logo20.createTexture("icon/����ͼ��/�綯Һѹǯ��.jpg");
		logo20_Id = (GLuint *)logo20.textureId;

		ImageTexture logo21;
		logo21.createTexture("icon/����ͼ��/�Զ��ӻ�װ��1_֧�߼н�_��.jpg");
		logo21_Id = (GLuint *)logo21.textureId;

		ImageTexture logo22;
		logo22.createTexture("icon/����ͼ��/�Զ��ӻ�װ��1_֧�߼н�_��.jpg");
		logo22_Id = (GLuint *)logo22.textureId;

		ImageTexture logo23;
		logo23.createTexture("icon/����ͼ��/�Զ��ӻ�װ��1_š���߼�_��.jpg");
		logo23_Id = (GLuint *)logo23.textureId;

		ImageTexture logo24;
		logo24.createTexture("icon/����ͼ��/�Զ��ӻ�װ��1_š���߼�_��.jpg");
		logo24_Id = (GLuint *)logo24.textureId;

		ImageTexture logo25;
		logo25.createTexture("icon/����ͼ��/�Զ��ӻ�װ��2_֧�߼н�_��.jpg");
		logo25_Id = (GLuint *)logo25.textureId;

		ImageTexture logo26;
		logo26.createTexture("icon/����ͼ��/�Զ��ӻ�װ��2_֧�߼н�_��.jpg");
		logo26_Id = (GLuint *)logo26.textureId;

		ImageTexture logo27;
		logo27.createTexture("icon/����ͼ��/�Զ��ӻ�װ��2_š���߼�_��.jpg");
		logo27_Id = (GLuint *)logo27.textureId;

		ImageTexture logo28;
		logo28.createTexture("icon/����ͼ��/�Զ��ӻ�װ��2_š���߼�_��.jpg");
		logo28_Id = (GLuint *)logo28.textureId;

		ImageTexture logo29;
		logo29.createTexture("icon/����ͼ��/�Զ��ӻ�װ��3_֧�߼н�_��.jpg");
		logo29_Id = (GLuint *)logo29.textureId;

		ImageTexture logo30;
		logo30.createTexture("icon/����ͼ��/�Զ��ӻ�װ��3_֧�߼н�_��.jpg");
		logo30_Id = (GLuint *)logo30.textureId;

		ImageTexture logo31;
		logo31.createTexture("icon/����ͼ��/�Զ��ӻ�װ��3_š���߼�_��.jpg");
		logo31_Id = (GLuint *)logo31.textureId;

		ImageTexture logo32;
		logo32.createTexture("icon/����ͼ��/�Զ��ӻ�װ��3_š���߼�_��.jpg");
		logo32_Id = (GLuint *)logo32.textureId;

		ImageTexture logo33;
		logo33.createTexture("icon/����ͼ��/���ӳ���.jpg");
		logo33_Id = (GLuint *)logo33.textureId;

		ImageTexture logo34;
		logo34.createTexture("icon/����ͼ��/���ӳ�俪.jpg");
		logo34_Id = (GLuint *)logo34.textureId;

		ImageTexture logo35;
		logo35.createTexture("icon/����ͼ��/�ұ�ӳ���.jpg");
		logo35_Id = (GLuint *)logo35.textureId;

		ImageTexture logo36;
		logo36.createTexture("icon/����ͼ��/�ұ�ӳ�俪.jpg");
		logo36_Id = (GLuint *)logo36.textureId;



		/********************************����ͼ��***********************************/
		ImageTexture direction_forward;
		direction_forward.createTexture("icon/����ͼ��/ǰ.jpg");
		direction_forward_id = (GLuint*)direction_forward.textureId;

		ImageTexture direction_backward;
		direction_backward.createTexture("icon/����ͼ��/��.jpg");
		direction_backward_id = (GLuint*)direction_backward.textureId;

		ImageTexture direction_left;
		direction_left.createTexture("icon/����ͼ��/��.jpg");
		direction_left_id = (GLuint*)direction_left.textureId;

		ImageTexture direction_right;
		direction_right.createTexture("icon/����ͼ��/��.jpg");
		direction_right_id = (GLuint*)direction_right.textureId;

		ImageTexture direction_up;
		direction_up.createTexture("icon/����ͼ��/��.jpg");
		direction_up_id = (GLuint*)direction_up.textureId;

		ImageTexture direction_down;
		direction_down.createTexture("icon/����ͼ��/��.jpg");
		direction_down_id = (GLuint*)direction_down.textureId;

		/***************** *********��֧����·�����ڲ�ͼ��******************************/
		ImageTexture function_01_01;
		function_01_01.createTexture("icon/������ҵ/��֧����·����/ң��������.jpg");
		function_01_01_id = (GLuint*)function_01_01.textureId;

		ImageTexture function_01_02;
		function_01_02.createTexture("icon/������ҵ/��֧����·����/�Զ�����.jpg");
		function_01_02_id = (GLuint*)function_01_02.textureId;

		ImageTexture function_01_03;
		function_01_03.createTexture("icon/������ҵ/��֧����·����/�ұۻ�����λ��.jpg");
		function_01_03_id = (GLuint*)function_01_03.textureId;

		ImageTexture function_01_04;
		function_01_04.createTexture("icon/������ҵ/��֧����·����/ȡ����1����ץ��.jpg");
		function_01_04_id = (GLuint*)function_01_04.textureId;

		ImageTexture function_01_05;
		function_01_05.createTexture("icon/������ҵ/��֧����·����/ȡ4��λ�ӻ�װ��.jpg");
		function_01_05_id = (GLuint*)function_01_05.textureId;

		ImageTexture function_01_06;
		function_01_06.createTexture("icon/������ҵ/��֧����·����/���ץ��λ��.jpg");
		function_01_06_id = (GLuint*)function_01_06.textureId;

		ImageTexture function_01_07;
		function_01_07.createTexture("icon/������ҵ/��֧����·����/���ץ�ֽӻ�λ��.jpg");
		function_01_07_id = (GLuint*)function_01_07.textureId;

		ImageTexture function_01_08;
		function_01_08.createTexture("icon/������ҵ/��֧����·����/���ʶ��.jpg");
		function_01_08_id = (GLuint*)function_01_08.textureId;

		ImageTexture function_01_09;
		function_01_09.createTexture("icon/������ҵ/��֧����·����/����0.5.jpg");
		function_01_09_id = (GLuint*)function_01_09.textureId;

		ImageTexture function_01_10;
		function_01_10.createTexture("icon/������ҵ/��֧����·����/����0.3.jpg");
		function_01_10_id = (GLuint*)function_01_10.textureId;

		ImageTexture function_01_11;
		function_01_11.createTexture("icon/������ҵ/��֧����·����/����0.2.jpg");
		function_01_11_id = (GLuint*)function_01_11.textureId;

		ImageTexture function_01_12;
		function_01_12.createTexture("icon/������ҵ/��֧����·����/����0.1.jpg");
		function_01_12_id = (GLuint*)function_01_12.textureId;

		ImageTexture function_01_13;
		function_01_13.createTexture("icon/������ҵ/��֧����·����/����0.jpg");
		function_01_13_id = (GLuint*)function_01_13.textureId;

		ImageTexture function_01_14;
		function_01_14.createTexture("icon/������ҵ/��֧����·����/����-0.1.jpg");
		function_01_14_id = (GLuint*)function_01_14.textureId;

		ImageTexture function_01_15;
		function_01_15.createTexture("icon/������ҵ/��֧����·����/��ۺ�.jpg");
		function_01_15_id = (GLuint*)function_01_15.textureId;

		ImageTexture function_01_16;
		function_01_16.createTexture("icon/������ҵ/��֧����·����/����-0.1.jpg");
		function_01_16_id = (GLuint*)function_01_16.textureId;

		ImageTexture function_01_17;
		function_01_17.createTexture("icon/������ҵ/��֧����·����/����.jpg");
		function_01_17_id = (GLuint*)function_01_17.textureId;

		ImageTexture function_01_18;
		function_01_18.createTexture("icon/������ҵ/��֧����·����/����0.1.jpg");
		function_01_18_id = (GLuint*)function_01_18.textureId;

		ImageTexture function_01_19;
		function_01_19.createTexture("icon/������ҵ/��֧����·����/����0.2.jpg");
		function_01_19_id = (GLuint*)function_01_19.textureId;

		ImageTexture function_01_20;
		function_01_20.createTexture("icon/������ҵ/��֧����·����/���¸�λ.jpg");
		function_01_20_id = (GLuint*)function_01_20.textureId;

		ImageTexture function_01_21;
		function_01_21.createTexture("icon/������ҵ/��֧����·����/�ұ۽ӻ�װ�ø�λ.jpg");
		function_01_21_id = (GLuint*)function_01_21.textureId;

		ImageTexture function_01_22;
		function_01_22.createTexture("icon/������ҵ/��֧����·����/��4��λ�ӻ�װ��.jpg");
		function_01_22_id = (GLuint*)function_01_22.textureId;

		ImageTexture function_01_23;
		function_01_23.createTexture("icon/������ҵ/��֧����·����/�Ź���1����ץ��.jpg");
		function_01_23_id = (GLuint*)function_01_23.textureId;

		ImageTexture function_01_24;
		function_01_24.createTexture("icon/������ҵ/��֧����·����/���ؾ�����ҵ.jpg");
		function_01_24_id = (GLuint*)function_01_24.textureId;

		ImageTexture function_01_25;
		function_01_25.createTexture("icon/������ҵ/��֧����·����/�ӻ����.jpg");
		function_01_25_id = (GLuint*)function_01_25.textureId;

		ImageTexture function_01_26;
		function_01_26.createTexture("icon/������ҵ/��֧����·����/��۳�ʼλ��.jpg");
		function_01_26_id = (GLuint*)function_01_26.textureId;



	}

}