#pragma once

#include <QWidget>
class QStackedWidget;

namespace Ui { class PageContainer; };

class PageContainer : public QWidget
{
	Q_OBJECT

public:
	PageContainer(QWidget *parent = Q_NULLPTR);
	~PageContainer();

private:
	Ui::PageContainer *ui;
	QStackedWidget *stackedWidget;

private slots:
	void changePage(int n);

};
