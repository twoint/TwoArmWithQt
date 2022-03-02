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

	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//ȥ���ڱ߿�
	setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��

	//��3��radio button����Ϊһ�飬Ϊ��ȡѡ��ֵ��׼��
	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //���������ť��Ϊ����ģʽ
	qButtonGroup->addButton(ui.radioButton_dubug_mode, 0);
	qButtonGroup->addButton(ui.radioButton_work_mode, 1);
	qButtonGroup->addButton(ui.radioButton_service_mode, 2);

	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(on_btn_confirm()));
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
}

/**********����radio button,ѡ����Ӧģʽ************/
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