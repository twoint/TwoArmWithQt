#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);
	void ThreadShow(const QString& str);//调试用，在其它线程中向ui控件中添加文本

private:
	Ui::DebugModeClass ui;


private slots:
	void on_btn_exit();
};
