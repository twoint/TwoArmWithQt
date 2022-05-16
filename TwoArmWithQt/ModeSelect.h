#pragma once
#include <qwidget.h>
#include"ui_ModeSelect.h"
#include<qbuttongroup.h>

class ModeSelect :
	public QWidget
{
	Q_OBJECT

public:
	ModeSelect(QWidget *parent = Q_NULLPTR);

private:
	Ui::ModeSelectClass ui;
	QButtonGroup* qButtonGroup;
	int number;//选中的场景

signals:
	void jumpPageTo(int n);


private slots:
	void on_btn_confirm();//确认按钮,进入相应模式

};

