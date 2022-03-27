#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"
#include"qdom.h"
#include<qmessagebox.h>
#include<QFile>
#include"ModeSelect.h"

/********解决弹窗中文乱码问题**************/
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
	Ui::LoginClass ui;//当前登录界面
	QDomDocument mydoc;//xml文件

signals:
	void jumpPageTo(int n);

private slots:
	void on_btn_login();//登录按钮操作
};