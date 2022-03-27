#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"
#include"qdom.h"
#include<qmessagebox.h>
#include<QFile>
#include"ModeSelect.h"

/********�������������������**************/
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif
/****************************************/

class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);

private:
	Ui::LoginClass ui;//��ǰ��¼����
	QDomDocument mydoc;//xml�ļ�

signals:
	void jumpPageTo(int n);

private slots:
	void on_btn_login();//��¼��ť����
};