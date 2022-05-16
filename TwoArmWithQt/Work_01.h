#pragma once

#include <QWidget>
#include "ui_Work_01.h"

class Work_01 : public QWidget
{
	Q_OBJECT

public:
	Work_01(QWidget *parent = Q_NULLPTR);
	~Work_01();

signals:
	void jumpPageTo(int n);

private:
	Ui::Work_01 ui;

private slots:
	void on_btn_exit();

};
