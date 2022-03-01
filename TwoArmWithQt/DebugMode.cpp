#include "DebugMode.h"

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//去窗口边框
	setAttribute(Qt::WA_TranslucentBackground);							//把窗口背景设置为透明

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
}

void DebugMode::on_btn_exit()
{
	this->close();
}

//git测试