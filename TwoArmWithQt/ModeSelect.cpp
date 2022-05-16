#include "ModeSelect.h"
#include"DebugMode.h"
#include"WorkMode.h"
#include"ServeMode.h"

ModeSelect::ModeSelect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//将3个radio button设置为一组，为读取选中值做准备
	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //设置这个按钮组为互斥模式
	qButtonGroup->addButton(ui.radioButton_dubug_mode, 0);
	qButtonGroup->addButton(ui.radioButton_work_mode, 1);
	qButtonGroup->addButton(ui.radioButton_service_mode, 2);

	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(on_btn_confirm()));
}

/**********根据radio button,选择相应模式************/
void ModeSelect::on_btn_confirm()
{
	number = qButtonGroup->checkedId();
	//解决按钮选中残留问题，将选中的按钮设置为未选中状态
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

