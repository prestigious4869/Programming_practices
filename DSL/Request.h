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
	void getfile();//��ȡ�û���Request���ڵ����벢����
	string input;//����û���Request���ڵ�����
	DSL* dialog = NULL;//����DSL���ڶ���

private:
	Ui::Request request;
	
};
