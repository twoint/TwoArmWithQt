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

signals:
	void jumpPageTo(int n);

private slots:
	void on_btn_exit();

};