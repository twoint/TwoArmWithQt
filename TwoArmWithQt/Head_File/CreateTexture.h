#pragma once
#include "cvTest.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "ImageTexture.h"

extern ToolConnect ToolConnectCom;
extern RobotArm Robot;

//图片id
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
GLuint Work_01_ID;//接支接线路引线内部显示图片



//功能图标纹理id
ImTextureID function_logo_01_Id;
ImTextureID function_logo_02_Id;
ImTextureID function_logo_03_Id;
ImTextureID function_logo_04_Id;
ImTextureID function_logo_05_Id;
ImTextureID function_logo_06_Id;
ImTextureID function_logo_07_Id;
ImTextureID function_logo_08_Id;
ImTextureID function_logo_09_Id;

//方向图标纹理id
ImTextureID direction_forward_id;
ImTextureID direction_backward_id;
ImTextureID direction_left_id;
ImTextureID direction_right_id;
ImTextureID direction_up_id;
ImTextureID direction_down_id;

//工具图标纹理id
ImTextureID logo07_Id;//左臂网络
ImTextureID logo08_Id;
ImTextureID logo09_Id;//右臂网络
ImTextureID logo10_Id;
ImTextureID logo11_Id;//引线抓手
ImTextureID logo12_Id;
ImTextureID logo13_Id;//避雷器抓手
ImTextureID logo14_Id;
ImTextureID logo15_Id;//电动扳手正转
ImTextureID logo16_Id;
ImTextureID logo17_Id;//电动扳手反转
ImTextureID logo18_Id;
ImTextureID logo19_Id;//电动液压钳
ImTextureID logo20_Id;
ImTextureID logo21_Id;//自动接火装置1_支线夹紧
ImTextureID logo22_Id;
ImTextureID logo23_Id;//自动接火装置1_拧紧线夹
ImTextureID logo24_Id;
ImTextureID logo25_Id;//自动接火装置2_支线夹紧
ImTextureID logo26_Id;
ImTextureID logo27_Id;//自动接火装置2_拧紧线夹
ImTextureID logo28_Id;
ImTextureID logo29_Id;//自动接火装置3_支线夹紧
ImTextureID logo30_Id;
ImTextureID logo31_Id;//自动接火装置3_拧紧线夹
ImTextureID logo32_Id;
ImTextureID logo33_Id;//左臂映射
ImTextureID logo34_Id;
ImTextureID logo35_Id;//右臂映射
ImTextureID logo36_Id;

//打开关闭图标纹理id
ImTextureID OpenImageID;
ImTextureID CloseImageID;

//接支接线路引线内部图片纹理
ImTextureID function_01_01_id;//遥操作控制
ImTextureID function_01_02_id;//自动控制
ImTextureID function_01_03_id;//右臂换工具位置
ImTextureID function_01_04_id;//取工具1引线抓手
ImTextureID function_01_05_id;//取4号位接火装置
ImTextureID function_01_06_id;//左臂抓线位置
ImTextureID function_01_07_id;//左臂抓手接火位置
ImTextureID function_01_08_id;//相机识别
ImTextureID function_01_09_id;//穿线0.5
ImTextureID function_01_10_id;//穿线0.3
ImTextureID function_01_11_id;//穿线0.2
ImTextureID function_01_12_id;//穿线0.1
ImTextureID function_01_13_id;//穿线0
ImTextureID function_01_14_id;//穿线-0.1
ImTextureID function_01_15_id;//左臂后撤
ImTextureID function_01_16_id;//旋举-0.1
ImTextureID function_01_17_id;//旋举
ImTextureID function_01_18_id;//旋举0.1
ImTextureID function_01_19_id;//旋举0.2
ImTextureID function_01_20_id;//放下复位
ImTextureID function_01_21_id;//右臂接火装置复位
ImTextureID function_01_22_id;//放4号位接火装置
ImTextureID function_01_23_id;//放工具1引线抓手
ImTextureID function_01_24_id;//返回九项作业
ImTextureID function_01_25_id;//接火完成
ImTextureID function_01_26_id;//左臂初始位置







//创建图片纹理
void ImageCreateTexture(const String& m_fileName, GLuint& textureId)
{
	cv::Mat img = imread(m_fileName);
	if (img.empty())
	{
		printf("Can not load image %s\n", m_fileName);
	}
	else
	{
		//设置长宽
		int width = img.cols;
		int height = img.rows;
		int channel = img.channels();
		//获取图像指针
		int pixellength = width * height * channel;
		GLubyte* pixels = new GLubyte[pixellength];
		memcpy(pixels, img.data, pixellength * sizeof(char));
		// 使用GL指令生成贴图，获取贴图ID
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		//必须一个RGB  一个BGR（opencv的mat类的颜色通道是BGR） 否则会出现颜色偏差
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		//纹理放大缩小使用线性插值
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		free(pixels);
	}
}
//创建相机纹理
void CameraCreateTexture(cv::VideoCapture vcap, GLuint id)
{
	static cv::Mat image;
	if (!vcap.read(image))
	{
		printf("Can not load Cameras\n");
	}
	else
	{
		//设置长宽

		Size dsize = Size(1080, 720);
		//Mat img2 = Mat(dsize, CV_32S);
		resize(image, image, dsize);
		int width = image.cols;
		int height = image.rows;
		int channel = image.channels();
		//获取图像指针
		int pixellength = width * height * channel;
		cout << " ******************************************" << endl;
		cout << width << endl;
		static GLubyte* rtsppixels = new GLubyte[pixellength];
		memcpy(rtsppixels, image.data, pixellength * sizeof(char));

		glBindTexture(GL_TEXTURE_2D, id);
		//必须一个RGB  一个BGR（opencv的mat类的颜色通道是BGR） 否则会出现颜色偏差
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, rtsppixels);
		//纹理放大缩小使用线性插值
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		//free(pixels);
		///image.release();
		//delete []pixels;
	}
}
//创建vrep纹理
void VrepCreateTexture(simxUChar* vrep_image)
{
	static cv::Mat image;
	static cv::Mat image_fliped;//需要重新赋予新的Mat变量，否则会存在没有成功翻转的情况
	image = cv::Mat(Robot.resolution[0], Robot.resolution[1], CV_8UC3, vrep_image);//读回来的图像数据是垂直翻转的,问题应该是在cvMat 和 v-rep 垂直坐标轴的方向相反,flip一下就正常了

	cv::flip(image, image_fliped, 0);
	//读回来的图像数据时rgb通道分布的，而cvMat 默认bgr
	cv::cvtColor(image_fliped, image_fliped, cv::COLOR_RGB2BGR);
	if (image_fliped.empty())
	{
		printf("Can not load Cameras\n");
	}
	else
	{
		//设置长宽
		int width = image_fliped.cols;
		int height = image_fliped.rows;
		int channel = image_fliped.channels();

		//获取图像指针
		int pixellength = width * height * channel;
		static GLubyte* pixels = new GLubyte[pixellength];
		memcpy(pixels, image_fliped.data, pixellength * sizeof(char));
		glBindTexture(GL_TEXTURE_2D, VrepID);

		//必须一个RGB  一个BGR（opencv的mat类的颜色通道是BGR） 否则会出现颜色偏差
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
		//纹理放大缩小使用线性插值
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		/*image.release();
		image_fliped.release();*/
		//free(pixels);
	}
}
//纹理初始化
void textureInit() {
	static bool initFlag = false;
	if (!initFlag)
	{
		initFlag = true;

		ImageCreateTexture("icon/图片/国家电网.jpg", ImageID);
		ImageCreateTexture("icon/图片/相机.jpg", CameraLogoID);
		ImageCreateTexture("icon/图片/作业.jpg", FunctionImageID);
		ImageCreateTexture("icon/图片/vrep图像.jpg", VrepLogoID);
		ImageCreateTexture("icon/图片/工具.jpg", ToolImageID);
		ImageCreateTexture("icon/图片/摄像头关闭.jpg", CameraCloseImageID);
		ImageCreateTexture("icon/图片/左臂遥操作.jpg", LeftControlImageID);
		ImageCreateTexture("icon/图片/右臂遥操作.jpg", RightControlImageID);
		ImageCreateTexture("icon/各项作业/接支接线路引线/接支接线路引线.jpg", Work_01_ID);



		ImageTexture openLogo;
		openLogo.createTexture("icon/图片/打开.jpg");
		OpenImageID = (GLuint *)openLogo.textureId;

		ImageTexture closeLogo;
		closeLogo.createTexture("icon/图片/关闭.jpg");
		CloseImageID = (GLuint *)closeLogo.textureId;

		/********************************功能图标***********************************/
		ImageTexture function_logo_01;
		function_logo_01.createTexture("icon/功能图标/接支接线路引线.jpg");
		function_logo_01_Id = (GLuint *)function_logo_01.textureId;

		ImageTexture function_logo_02;
		function_logo_02.createTexture("icon/功能图标/断支接线路引线.jpg");
		function_logo_02_Id = (GLuint *)function_logo_02.textureId;

		ImageTexture function_logo_03;
		function_logo_03.createTexture("icon/功能图标/更换避雷器.jpg");
		function_logo_03_Id = (GLuint *)function_logo_03.textureId;

		ImageTexture function_logo_04;
		function_logo_04.createTexture("icon/功能图标/接跌落式熔断器上引线.jpg");
		function_logo_04_Id = (GLuint *)function_logo_04.textureId;

		ImageTexture function_logo_05;
		function_logo_05.createTexture("icon/功能图标/断跌落式熔断器上引线.jpg");
		function_logo_05_Id = (GLuint *)function_logo_05.textureId;

		ImageTexture function_logo_06;
		function_logo_06.createTexture("icon/功能图标/更换跌落式熔断器.jpg");
		function_logo_06_Id = (GLuint *)function_logo_06.textureId;

		ImageTexture function_logo_07;
		function_logo_07.createTexture("icon/功能图标/装故障指示器.jpg");
		function_logo_07_Id = (GLuint *)function_logo_07.textureId;

		ImageTexture function_logo_08;
		function_logo_08.createTexture("icon/功能图标/拆故障指示器.jpg");
		function_logo_08_Id = (GLuint *)function_logo_08.textureId;

		ImageTexture function_logo_09;
		function_logo_09.createTexture("icon/功能图标/除枝.jpg");
		function_logo_09_Id = (GLuint *)function_logo_09.textureId;


		/********************************工具图标***********************************/
		ImageTexture logo07;
		logo07.createTexture("icon/工具图标/左臂网络关.jpg");
		logo07_Id = (GLuint *)logo07.textureId;

		ImageTexture logo08;
		logo08.createTexture("icon/工具图标/左臂网络开.jpg");
		logo08_Id = (GLuint *)logo08.textureId;

		ImageTexture logo09;
		logo09.createTexture("icon/工具图标/右臂网络关.jpg");
		logo09_Id = (GLuint *)logo09.textureId;

		ImageTexture logo10;
		logo10.createTexture("icon/工具图标/右臂网络开.jpg");
		logo10_Id = (GLuint *)logo10.textureId;

		ImageTexture logo11;
		logo11.createTexture("icon/工具图标/引线抓手关.jpg");
		logo11_Id = (GLuint *)logo11.textureId;

		ImageTexture logo12;
		logo12.createTexture("icon/工具图标/引线抓手开.jpg");
		logo12_Id = (GLuint *)logo12.textureId;

		ImageTexture logo13;
		logo13.createTexture("icon/工具图标/避雷器抓手关.jpg");
		logo13_Id = (GLuint *)logo13.textureId;

		ImageTexture logo14;
		logo14.createTexture("icon/工具图标/避雷器抓手开.jpg");
		logo14_Id = (GLuint *)logo14.textureId;

		ImageTexture logo15;
		logo15.createTexture("icon/工具图标/电动扳手正转关.jpg");
		logo15_Id = (GLuint *)logo15.textureId;

		ImageTexture logo16;
		logo16.createTexture("icon/工具图标/电动扳手正转开.jpg");
		logo16_Id = (GLuint *)logo16.textureId;

		ImageTexture logo17;
		logo17.createTexture("icon/工具图标/电动扳手反转关.jpg");
		logo17_Id = (GLuint *)logo17.textureId;

		ImageTexture logo18;
		logo18.createTexture("icon/工具图标/电动扳手反转开.jpg");
		logo18_Id = (GLuint *)logo18.textureId;

		ImageTexture logo19;
		logo19.createTexture("icon/工具图标/电动液压钳关.jpg");
		logo19_Id = (GLuint *)logo19.textureId;

		ImageTexture logo20;
		logo20.createTexture("icon/工具图标/电动液压钳开.jpg");
		logo20_Id = (GLuint *)logo20.textureId;

		ImageTexture logo21;
		logo21.createTexture("icon/工具图标/自动接火装置1_支线夹紧_关.jpg");
		logo21_Id = (GLuint *)logo21.textureId;

		ImageTexture logo22;
		logo22.createTexture("icon/工具图标/自动接火装置1_支线夹紧_开.jpg");
		logo22_Id = (GLuint *)logo22.textureId;

		ImageTexture logo23;
		logo23.createTexture("icon/工具图标/自动接火装置1_拧紧线夹_关.jpg");
		logo23_Id = (GLuint *)logo23.textureId;

		ImageTexture logo24;
		logo24.createTexture("icon/工具图标/自动接火装置1_拧紧线夹_开.jpg");
		logo24_Id = (GLuint *)logo24.textureId;

		ImageTexture logo25;
		logo25.createTexture("icon/工具图标/自动接火装置2_支线夹紧_关.jpg");
		logo25_Id = (GLuint *)logo25.textureId;

		ImageTexture logo26;
		logo26.createTexture("icon/工具图标/自动接火装置2_支线夹紧_开.jpg");
		logo26_Id = (GLuint *)logo26.textureId;

		ImageTexture logo27;
		logo27.createTexture("icon/工具图标/自动接火装置2_拧紧线夹_关.jpg");
		logo27_Id = (GLuint *)logo27.textureId;

		ImageTexture logo28;
		logo28.createTexture("icon/工具图标/自动接火装置2_拧紧线夹_开.jpg");
		logo28_Id = (GLuint *)logo28.textureId;

		ImageTexture logo29;
		logo29.createTexture("icon/工具图标/自动接火装置3_支线夹紧_关.jpg");
		logo29_Id = (GLuint *)logo29.textureId;

		ImageTexture logo30;
		logo30.createTexture("icon/工具图标/自动接火装置3_支线夹紧_开.jpg");
		logo30_Id = (GLuint *)logo30.textureId;

		ImageTexture logo31;
		logo31.createTexture("icon/工具图标/自动接火装置3_拧紧线夹_关.jpg");
		logo31_Id = (GLuint *)logo31.textureId;

		ImageTexture logo32;
		logo32.createTexture("icon/工具图标/自动接火装置3_拧紧线夹_开.jpg");
		logo32_Id = (GLuint *)logo32.textureId;

		ImageTexture logo33;
		logo33.createTexture("icon/工具图标/左臂映射关.jpg");
		logo33_Id = (GLuint *)logo33.textureId;

		ImageTexture logo34;
		logo34.createTexture("icon/工具图标/左臂映射开.jpg");
		logo34_Id = (GLuint *)logo34.textureId;

		ImageTexture logo35;
		logo35.createTexture("icon/工具图标/右臂映射关.jpg");
		logo35_Id = (GLuint *)logo35.textureId;

		ImageTexture logo36;
		logo36.createTexture("icon/工具图标/右臂映射开.jpg");
		logo36_Id = (GLuint *)logo36.textureId;



		/********************************方向图标***********************************/
		ImageTexture direction_forward;
		direction_forward.createTexture("icon/方向图标/前.jpg");
		direction_forward_id = (GLuint*)direction_forward.textureId;

		ImageTexture direction_backward;
		direction_backward.createTexture("icon/方向图标/后.jpg");
		direction_backward_id = (GLuint*)direction_backward.textureId;

		ImageTexture direction_left;
		direction_left.createTexture("icon/方向图标/左.jpg");
		direction_left_id = (GLuint*)direction_left.textureId;

		ImageTexture direction_right;
		direction_right.createTexture("icon/方向图标/右.jpg");
		direction_right_id = (GLuint*)direction_right.textureId;

		ImageTexture direction_up;
		direction_up.createTexture("icon/方向图标/上.jpg");
		direction_up_id = (GLuint*)direction_up.textureId;

		ImageTexture direction_down;
		direction_down.createTexture("icon/方向图标/下.jpg");
		direction_down_id = (GLuint*)direction_down.textureId;

		/***************** *********接支接线路引线内部图标******************************/
		ImageTexture function_01_01;
		function_01_01.createTexture("icon/各项作业/接支接线路引线/遥操作控制.jpg");
		function_01_01_id = (GLuint*)function_01_01.textureId;

		ImageTexture function_01_02;
		function_01_02.createTexture("icon/各项作业/接支接线路引线/自动控制.jpg");
		function_01_02_id = (GLuint*)function_01_02.textureId;

		ImageTexture function_01_03;
		function_01_03.createTexture("icon/各项作业/接支接线路引线/右臂换工具位置.jpg");
		function_01_03_id = (GLuint*)function_01_03.textureId;

		ImageTexture function_01_04;
		function_01_04.createTexture("icon/各项作业/接支接线路引线/取工具1引线抓手.jpg");
		function_01_04_id = (GLuint*)function_01_04.textureId;

		ImageTexture function_01_05;
		function_01_05.createTexture("icon/各项作业/接支接线路引线/取4号位接火装置.jpg");
		function_01_05_id = (GLuint*)function_01_05.textureId;

		ImageTexture function_01_06;
		function_01_06.createTexture("icon/各项作业/接支接线路引线/左臂抓线位置.jpg");
		function_01_06_id = (GLuint*)function_01_06.textureId;

		ImageTexture function_01_07;
		function_01_07.createTexture("icon/各项作业/接支接线路引线/左臂抓手接火位置.jpg");
		function_01_07_id = (GLuint*)function_01_07.textureId;

		ImageTexture function_01_08;
		function_01_08.createTexture("icon/各项作业/接支接线路引线/相机识别.jpg");
		function_01_08_id = (GLuint*)function_01_08.textureId;

		ImageTexture function_01_09;
		function_01_09.createTexture("icon/各项作业/接支接线路引线/穿线0.5.jpg");
		function_01_09_id = (GLuint*)function_01_09.textureId;

		ImageTexture function_01_10;
		function_01_10.createTexture("icon/各项作业/接支接线路引线/穿线0.3.jpg");
		function_01_10_id = (GLuint*)function_01_10.textureId;

		ImageTexture function_01_11;
		function_01_11.createTexture("icon/各项作业/接支接线路引线/穿线0.2.jpg");
		function_01_11_id = (GLuint*)function_01_11.textureId;

		ImageTexture function_01_12;
		function_01_12.createTexture("icon/各项作业/接支接线路引线/穿线0.1.jpg");
		function_01_12_id = (GLuint*)function_01_12.textureId;

		ImageTexture function_01_13;
		function_01_13.createTexture("icon/各项作业/接支接线路引线/穿线0.jpg");
		function_01_13_id = (GLuint*)function_01_13.textureId;

		ImageTexture function_01_14;
		function_01_14.createTexture("icon/各项作业/接支接线路引线/穿线-0.1.jpg");
		function_01_14_id = (GLuint*)function_01_14.textureId;

		ImageTexture function_01_15;
		function_01_15.createTexture("icon/各项作业/接支接线路引线/左臂后撤.jpg");
		function_01_15_id = (GLuint*)function_01_15.textureId;

		ImageTexture function_01_16;
		function_01_16.createTexture("icon/各项作业/接支接线路引线/旋举-0.1.jpg");
		function_01_16_id = (GLuint*)function_01_16.textureId;

		ImageTexture function_01_17;
		function_01_17.createTexture("icon/各项作业/接支接线路引线/旋举.jpg");
		function_01_17_id = (GLuint*)function_01_17.textureId;

		ImageTexture function_01_18;
		function_01_18.createTexture("icon/各项作业/接支接线路引线/旋举0.1.jpg");
		function_01_18_id = (GLuint*)function_01_18.textureId;

		ImageTexture function_01_19;
		function_01_19.createTexture("icon/各项作业/接支接线路引线/旋举0.2.jpg");
		function_01_19_id = (GLuint*)function_01_19.textureId;

		ImageTexture function_01_20;
		function_01_20.createTexture("icon/各项作业/接支接线路引线/放下复位.jpg");
		function_01_20_id = (GLuint*)function_01_20.textureId;

		ImageTexture function_01_21;
		function_01_21.createTexture("icon/各项作业/接支接线路引线/右臂接火装置复位.jpg");
		function_01_21_id = (GLuint*)function_01_21.textureId;

		ImageTexture function_01_22;
		function_01_22.createTexture("icon/各项作业/接支接线路引线/放4号位接火装置.jpg");
		function_01_22_id = (GLuint*)function_01_22.textureId;

		ImageTexture function_01_23;
		function_01_23.createTexture("icon/各项作业/接支接线路引线/放工具1引线抓手.jpg");
		function_01_23_id = (GLuint*)function_01_23.textureId;

		ImageTexture function_01_24;
		function_01_24.createTexture("icon/各项作业/接支接线路引线/返回九项作业.jpg");
		function_01_24_id = (GLuint*)function_01_24.textureId;

		ImageTexture function_01_25;
		function_01_25.createTexture("icon/各项作业/接支接线路引线/接火完成.jpg");
		function_01_25_id = (GLuint*)function_01_25.textureId;

		ImageTexture function_01_26;
		function_01_26.createTexture("icon/各项作业/接支接线路引线/左臂初始位置.jpg");
		function_01_26_id = (GLuint*)function_01_26.textureId;



	}

}