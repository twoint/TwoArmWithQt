#pragma once
#include <qwidget.h>
#include"ui_WorkMode.h"

class WorkMode :
	public QWidget
{
	Q_OBJECT

public:
	WorkMode(QWidget *parent = Q_NULLPTR);

signals:
	void jumpPageTo(int n);

private:
	Ui::WorkModeClass ui;
	QButtonGroup* qButtonGroup;
	int number;//选中的场景

private slots:
	void on_btn_exit();
	void on_btn_confirm();
};