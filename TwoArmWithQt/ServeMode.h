#pragma once
#include <qwidget.h>
#include"ui_ServeMode.h"

class ServeMode :
	public QWidget
{
	Q_OBJECT

public:
	ServeMode(QWidget *parent = Q_NULLPTR);

private:
	Ui::ServeModeClass ui;

};