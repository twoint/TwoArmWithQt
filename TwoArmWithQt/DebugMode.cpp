#include "DebugMode.h"
#include"ModeSelect.h"

extern ModeSelect* modeselect;


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
	modeselect->show();
	this->close();
}

void DebugMode::ThreadShow(const QString & str)
{
	ui.plainTextEdit->insertPlainText(str);
}


