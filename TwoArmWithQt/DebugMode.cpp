#include "DebugMode.h"

DebugMode::DebugMode(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | windowFlags());			//ȥ���ڱ߿�
	setAttribute(Qt::WA_TranslucentBackground);							//�Ѵ��ڱ�������Ϊ͸��

	connect(ui.btn_exit, SIGNAL(clicked()), this, SLOT(on_btn_exit()));
}

void DebugMode::on_btn_exit()
{
	this->close();
}

//git����