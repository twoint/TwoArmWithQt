#include "WorkMode.h"
#include"ModeSelect.h"
#include"Work_01.h"

extern ModeSelect* modeselect;
extern Work_01* work_01;

WorkMode::WorkMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //设置这个按钮组为互斥模式
	qButtonGroup->addButton(ui.radioButton_func_1, 0);
	qButtonGroup->addButton(ui.radioButton_func_2, 1);
	qButtonGroup->addButton(ui.radioButton_func_3, 2);
	qButtonGroup->addButton(ui.radioButton_func_4, 3);
	qButtonGroup->addButton(ui.radioButton_func_5, 4);
	qButtonGroup->addButton(ui.radioButton_func_6, 5);
	qButtonGroup->addButton(ui.radioButton_func_7, 6);
	qButtonGroup->addButton(ui.radioButton_func_8, 7);
	qButtonGroup->addButton(ui.radioButton_func_9, 8);

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	connect(ui.btn_confirm, SIGNAL(clicked()), this, SLOT(on_btn_confirm()));
}

void WorkMode::on_btn_exit()
{
	emit jumpPageTo(1);
}

/**********根据radio button,选择相应场景************/
void WorkMode::on_btn_confirm()
{
	number = qButtonGroup->checkedId();
	//解决按钮选中残留问题，将选中的按钮设置为未选中状态
	qButtonGroup->setExclusive(false);
	qButtonGroup->button(number)->setChecked(false);
	qButtonGroup->setExclusive(true);

	switch (number)
	{
	case 0:
		emit jumpPageTo(5);
		break;
	//case 1:
	//	emit jumpPageTo(3);
	//	break;
	//case 2:
	//	emit jumpPageTo(4);
	//	break;

	default:
		break;
	}
}