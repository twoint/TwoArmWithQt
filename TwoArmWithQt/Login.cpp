#include "Login.h"

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	connect(ui.btn_login, SIGNAL(clicked()), this, SLOT(on_btn_login()));
}


/*************登录*********** *****/
void Login::on_btn_login()
{
	QFile file("userlog.xml");
	mydoc.setContent(&file);
	QDomElement root = mydoc.documentElement();
	if (root.hasChildNodes())
	{
		QDomNodeList userList = root.childNodes();
		bool exist = false;
		for (int i = 0; i < userList.count(); i++)
		{
			QDomNode user = userList.at(i);
			QDomNodeList record = user.childNodes();

			QString uname = record.at(0).toElement().text();
			QString pword = record.at(1).toElement().text();
			if (uname == ui.lineEdit_username->text())
			{
				exist = true;
				if (!(pword == ui.lineEdit_password->text()))
				{
					QMessageBox::warning(0, QObject::tr("提示"), "口令错，请重新输入");
					ui.lineEdit_password->clear();
					ui.lineEdit_password->setFocus();
					return;
				}
			}
		}
		if (!exist)
		{
			QMessageBox::warning(0, QObject::tr("提示"), "此用户不存在");
			ui.lineEdit_username->clear();
			ui.lineEdit_password->clear();
			ui.lineEdit_password->setFocus();
			return;
		}
		emit jumpPageTo(1);


	}
}
