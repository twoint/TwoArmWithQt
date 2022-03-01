#pragma once
#include <qwidget.h>
#include"ui_WorkMode.h"

class WorkMode :
	public QWidget
{
	Q_OBJECT

public:
	WorkMode(QWidget *parent = Q_NULLPTR);

private:
	Ui::WorkModeClass ui;
};