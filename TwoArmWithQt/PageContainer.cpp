#include "PageContainer.h"
#include "ui_PageContainer.h"
#include"qstackedwidget.h"
#include"Login.h"
#include"ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"
#include"Work_01.h"

Login* login;
ModeSelect* modeselect;
DebugMode* debugmode;
WorkMode* workmode;
ServeMode* servemode;
Work_01* work_01;

PageContainer::PageContainer(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::PageContainer();
	ui->setupUi(this);
	setWindowTitle("配网带电作业机器人控制系统");
	setWindowIcon(QIcon(".\\picture\\robot.ico"));
	//去除最大最小化按钮
	setWindowFlags(Qt::Dialog);
	//固定窗口大小为1500*900
	setFixedSize(1500, 900);

	//初始化相关窗口
	stackedWidget = new QStackedWidget(this);
	stackedWidget->resize(1500, 900);

	login = new Login(this);
	modeselect = new ModeSelect(this);
	debugmode = new DebugMode(this);
	workmode = new WorkMode(this);
	servemode = new ServeMode(this);
	work_01 = new Work_01(this);

	//添加窗口进控件
	stackedWidget->addWidget(login);
	stackedWidget->addWidget(modeselect);
	stackedWidget->addWidget(debugmode);
	stackedWidget->addWidget(workmode);
	stackedWidget->addWidget(servemode);
	stackedWidget->addWidget(work_01);
	
	//调用当前窗口
	stackedWidget->setCurrentIndex(0);

	connect(login, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(modeselect, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(debugmode, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(workmode, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(servemode, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(work_01, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));

}

void PageContainer::changePage(int n)
{
	stackedWidget->setCurrentIndex(n);
}


PageContainer::~PageContainer()
{
	delete ui;
}
