#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Request.h"
#include "DSL.h"
using namespace std;

class Request : public QMainWindow
{
	Q_OBJECT

public:
	~Request();
	Request(QWidget *parent = Q_NULLPTR);
	void getfile();//获取用户在Request窗口的输入并分析
	string input;//存放用户在Request窗口的输入
	DSL* dialog = NULL;//创建DSL窗口对象

private:
	Ui::Request request;
	
};
