#include "ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"

ModeSelect::ModeSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//��3��radio button����Ϊһ�飬Ϊ��ȡѡ��ֵ��׼��
	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //���������ť��Ϊ����ģʽ
	qButtonGroup->addButton(ui.radioButton_dubug_mode, 0);
	qButtonGroup->addButton(ui.radioButton_work_mode, 1);
	qButtonGroup->addButton(ui.radioButton_service_mode, 2);

	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(on_btn_confirm()));
}

/**********����radio button,ѡ����Ӧģʽ************/
void ModeSelect::on_btn_confirm()
{
	number = qButtonGroup->checkedId();
	//�����ťѡ�в������⣬��ѡ�еİ�ť����Ϊδѡ��״̬
	qButtonGroup->setExclusive(false);
	qButtonGroup->button(number)->setChecked(false);
	qButtonGroup->setExclusive(true);

	switch (number)
	{
	case 0:

		emit jumpPageTo(2);
		break;
	case 1:
		emit jumpPageTo(3);
		break;
	case 2:
		emit jumpPageTo(4);
		break;

	default:
		break;
	}
}

