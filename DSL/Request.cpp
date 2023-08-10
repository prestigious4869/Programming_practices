#include "Request.h"
#include "DSL.h"

Request::Request(QWidget *parent)
	: QMainWindow(parent)
{
	/*����Request���ڣ�������������*/
	request.setupUi(this);
	request.lineEdit->setFont(QFont("songti", 12));
	request.lineEdit_2->setFont(QFont("kaiti", 12));
	/*�����ȷ������ȡ��������ת��getfile������*/
	connect(request.pushButton, &QPushButton::clicked, this, &Request::getfile);
	connect(request.pushButton_2, &QPushButton::clicked, this, &Request::getfile);
}

/*��ȡ�û�������Ϣ����������*/
void Request::getfile() {
	input = request.lineEdit->text().toStdString();
	string textfile = input;
	string testfile = "";
	this->close();

	if (textfile.empty()) {
		return;
	}
	/*
		����û������п�ͷ����auto��˵���û���Ҫ�Զ������ԣ�
		�����û��������ҵ������ı��Ͳ������ݵ��ļ�����
		ʹ����������������ʼ��DSL���ڡ�
	*/
	else if(input.find("auto") == 0) {
		string name = "";
		int i = 5;
		for (; input[i] != ' '; i++) {
			name += input[i];
		}
		textfile = name;
		testfile = input.substr(i + 1);
		dialog = new DSL(Q_NULLPTR, textfile, testfile);
		dialog->show();
	}
	/*���Զ�����������������DSL����*/
	else {
		dialog = new DSL(Q_NULLPTR, textfile, testfile);
		dialog->show();
	}
	/*ps����Ȼ������ͬһ��������delete������ֻ����getfile����������Ż���ʾDSL���ڣ���ֻ�������������ͷ�*/
}

Request::~Request()
{
}
