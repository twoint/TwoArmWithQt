#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);
	void ThreadShow(const QString& str);//�����ã��������߳�����ui�ؼ�������ı�

private:
	Ui::DebugModeClass ui;


private slots:
	void on_btn_exit();
};
