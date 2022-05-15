#include "ServeMode.h"

ServeMode::ServeMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
}

void ServeMode::on_btn_exit()
{
	emit jumpPageTo(1);
}
