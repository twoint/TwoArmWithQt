#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);

private:
	Ui::DebugModeClass ui;


private slots:
	void on_btn_exit();
};
