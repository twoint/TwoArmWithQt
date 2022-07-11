#pragma once
#include <qwidget.h>
#include"ui_DebugMode.h"
#include <QTimer>

class DebugMode :
	public QWidget
{
	Q_OBJECT

public:
	DebugMode(QWidget *parent = Q_NULLPTR);


private:
	Ui::DebugModeClass ui;
	QTimer *infoUpdateTimer;//传感器信息刷新

signals:
	void jumpPageTo(int n);

private slots:

	void infoUpdateFrame();	

};


