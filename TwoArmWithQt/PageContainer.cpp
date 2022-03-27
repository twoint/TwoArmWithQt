#include "PageContainer.h"
#include "ui_PageContainer.h"
#include"qstackedwidget.h"
#include"Login.h"
#include"ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"

Login* login;
ModeSelect* modeselect;
DebugMode* debugmode;
WorkMode* workmode;
ServeMode* servemode;


PageContainer::PageContainer(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::PageContainer();
	ui->setupUi(this);
	//ȥ�������С����ť
	setWindowFlags(Qt::Dialog);
	//�̶����ڴ�СΪ1500*900
	setFixedSize(1500, 900);

	//��ʼ����ش���
	stackedWidget = new QStackedWidget(this);
	stackedWidget->resize(1500, 900);

	login = new Login(this);
	modeselect = new ModeSelect(this);
	debugmode = new DebugMode(this);
	workmode = new WorkMode(this);
	servemode = new ServeMode(this);
	//��Ӵ��ڽ��ؼ�
	stackedWidget->addWidget(login);
	stackedWidget->addWidget(modeselect);
	stackedWidget->addWidget(debugmode);
	stackedWidget->addWidget(workmode);
	stackedWidget->addWidget(servemode);
	//���õ�ǰ����
	stackedWidget->setCurrentIndex(0);

	connect(login, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(modeselect, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));
	connect(debugmode, SIGNAL(jumpPageTo(int)), this, SLOT(changePage(int)));

}

void PageContainer::changePage(int n)
{
	stackedWidget->setCurrentIndex(n);
}


PageContainer::~PageContainer()
{
	delete ui;
}
