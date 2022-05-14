#include "WorkMode.h"
#include"ModeSelect.h"

extern ModeSelect* modeselect;

WorkMode::WorkMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	qButtonGroup = new QButtonGroup();
	qButtonGroup->setExclusive(true);								    //设置这个按钮组为互斥模式
	qButtonGroup->addButton(ui.radioButton_func_1, 0);
	qButtonGroup->addButton(ui.radioButton_func_2, 0);
	qButtonGroup->addButton(ui.radioButton_func_3, 0);
	qButtonGroup->addButton(ui.radioButton_func_4, 0);
	qButtonGroup->addButton(ui.radioButton_func_5, 0);
	qButtonGroup->addButton(ui.radioButton_func_6, 0);
	qButtonGroup->addButton(ui.radioButton_func_7, 0);
	qButtonGroup->addButton(ui.radioButton_func_8, 0);
	qButtonGroup->addButton(ui.radioButton_func_9, 0);

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));


}

void WorkMode::on_btn_exit()
{
	emit jumpPageTo(1);
}