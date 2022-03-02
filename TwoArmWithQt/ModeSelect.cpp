#include "ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"

extern DebugMode* debugmode;
extern WorkMode* workmode;
extern ServeMode* servemode;

ModeSelect::ModeSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//去窗口边框
	setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明

	//将3个radio button设置为一组，为读取选中值做准备
	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //设置这个按钮组为互斥模式
	qButtonGroup->addButton(ui.radioButton_dubug_mode, 0);
	qButtonGroup->addButton(ui.radioButton_work_mode, 1);
	qButtonGroup->addButton(ui.radioButton_service_mode, 2);

	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(on_btn_confirm()));
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
}

/**********根据radio button,选择相应模式************/
void ModeSelect::on_btn_confirm()
{
	number = qButtonGroup->checkedId();
	switch (number)
	{
	case 0:
		//debugmode = new DebugMode(0);
		debugmode->show();
		this->close();
		break;
	case 1:
		//workmode = new WorkMode(0);
		workmode->show();
		this->close();
		break;
	case 2:
		//servemode = new ServeMode(0);
		servemode->show();
		this->close();
		break;

	default:
		break;
	}
}

void ModeSelect::on_btn_exit()
{
	close();
}