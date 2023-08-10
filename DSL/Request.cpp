#include "Request.h"
#include "DSL.h"

Request::Request(QWidget *parent)
	: QMainWindow(parent)
{
	/*创建Request窗口，调整窗口字体*/
	request.setupUi(this);
	request.lineEdit->setFont(QFont("songti", 12));
	request.lineEdit_2->setFont(QFont("kaiti", 12));
	/*点击“确定”或“取消”，跳转到getfile函数中*/
	connect(request.pushButton, &QPushButton::clicked, this, &Request::getfile);
	connect(request.pushButton_2, &QPushButton::clicked, this, &Request::getfile);
}

/*获取用户输入信息，分析内容*/
void Request::getfile() {
	input = request.lineEdit->text().toStdString();
	string textfile = input;
	string testfile = "";
	this->close();

	if (textfile.empty()) {
		return;
	}
	/*
		如果用户输入中开头出现auto，说明用户需要自动化测试，
		则在用户输入中找到测试文本和测试数据的文件名，
		使用这两个变量来初始化DSL窗口。
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
	/*非自动化测试则正常创建DSL窗口*/
	else {
		dialog = new DSL(Q_NULLPTR, textfile, testfile);
		dialog->show();
	}
	/*ps：虽然很想在同一个函数中delete，但是只有在getfile函数运行完才会显示DSL窗口，故只能在主函数中释放*/
}

Request::~Request()
{
}
