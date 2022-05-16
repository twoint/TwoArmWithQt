#include "Work_01.h"

Work_01::Work_01(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
	
}

Work_01::~Work_01()
{
}

void Work_01::on_btn_exit()
{
	emit jumpPageTo(3);
}
