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
	int number;//ѡ�е�ģʽ  0������  1����ҵ   2��������

signals:
	void jumpPageTo(int n);


private slots:
	void on_btn_confirm();//ȷ�ϰ�ť,������Ӧģʽ

};

