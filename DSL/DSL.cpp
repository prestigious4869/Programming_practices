#include "DSL.h"
#include <iostream>
#include <QKeyEvent>
#include <sstream>
#include <iomanip>
#include <QTextCodec>
#include <fstream>
#include <windows.h>
using namespace std;

/*DSL窗口的构造函数*/
DSL::DSL(QWidget *parent, string text, string test)
    : QMainWindow(parent),textfile(text),inter(text),testfile(test)
{   
    dsl.setupUi(this);
    /*根据DSL程序修改窗口大小和robot_name*/
    if (inter.window_size.first >= 400 && inter.window_size.second >= 300) {
        this->resize(inter.window_size.first, inter.window_size.second);
    }
    if(inter.robot_name != "")
        robot_name = inter.robot_name;
    /*初始对话块应当为DSL程序中的第一个对话块*/
    if(inter.state.size() != 0)
        current_state = inter.state.begin()->first;

    /*设置DSL窗口的状态、字体、绑定信号使得用户发送后调用SendFromUser函数*/
    dsl.textEdit->setReadOnly(true);
    dsl.textEdit_2->setFont(QFont("songti", 12));
    dsl.textEdit_2->grabKeyboard();
    connect(dsl.pushButton, &QPushButton::clicked, this, &DSL::SendFromUser);

    /*DSL中如果有robot的默认回复，那么应该首先调用SendFromRobot*/
    for (int i = 0; i < inter.default_response.size(); i++) {
        SendFromRobot(inter.default_response[i]);
    }

    /*如果testfile不为空，说明用户采用的是自动化测试方式，需要从文件读入输入语句*/
    if (testfile != "") {
        ifstream f;
        f.open(testfile, ios::in);
        while (getline(f, user_send)) {
            SendFromUser();
        }
        f.close();
    }     
}

/*当用户按下ctrl+enter时发送对话，调用SendFromUser函数*/
void DSL::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        SendFromUser();
    }
}

/*获得用户在DSL窗口中的输入，将它显示在对话框中，并调用SendFromRobot*/
void DSL::SendFromUser() {
    QString str;
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    /*testfile不为空，则为自动化测试，直接从user_send获取到输入*/
    if (testfile != "") {
        str = pCodec->toUnicode(user_send.c_str(), user_send.length());
    }
    /*不为自动化测试，从用户输入框获取输入*/
    else {
        str = dsl.textEdit_2->toPlainText();
        user_send = pCodec->fromUnicode(str).data();
    }

    /*获取系统时间*/
    auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream ss;
    ss << put_time(localtime(&t), "%Y/%m/%d-%H:%M:%S");
    QString time = QString::fromStdString(ss.str());


    /*打印时间信息，并将其保持在对话框右侧*/
    QTextCharFormat fmt;
    fmt.setForeground(QColor("blue"));
    fmt.setFont(QFont("kaiti", 10));
    QTextCursor cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(time);
    dsl.textEdit->setAlignment(Qt::AlignRight);

    /*打印对话内容，并将其保持在对话框右侧*/
    fmt.setForeground(QColor("black"));
    fmt.setFont(QFont("songti", 15));
    cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(str);
    dsl.textEdit->setAlignment(Qt::AlignRight);

    /*将光标保持在用户输入框内*/
    cursor = dsl.textEdit_2->textCursor();
    dsl.textEdit_2->clear();
    dsl.textEdit_2->setFocus();

    /*调用Replystring函数获取所有输出*/
    vector<string> reply = ReplyString();
    for (int i = 0; i < reply.size(); i++) {
        if(reply[i]!="")
            SendFromRobot(reply[i]);
    }
}

/*调用ReplyString，并将回复内容显示在对话框中*/
void DSL::SendFromRobot(string str) {
    output.push_back(str);

    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    QString qstr = pCodec->toUnicode(str.c_str(), str.length());

    /*获取系统时间*/
    auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream ss;
    ss << put_time(localtime(&t), "%Y/%m/%d-%H:%M:%S");
    QString time = pCodec->toUnicode((ss.str() + " - " + this->robot_name).c_str(), (ss.str() + " - " + this->robot_name).length());

    /*打印时间信息，并将其保持在对话框左侧*/
    QTextCharFormat fmt;
    fmt.setForeground(QColor("blue"));
    fmt.setFont(QFont("kaiti", 10));
    QTextCursor cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(time);
    dsl.textEdit->setAlignment(Qt::AlignLeft);

    /*打印对话内容，并将其保持在对话框左侧*/
    fmt.setForeground(QColor("black"));
    fmt.setFont(QFont("songti", 15));
    cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(qstr);
    dsl.textEdit->setAlignment(Qt::AlignLeft);
}

vector<string> DSL::ReplyString() {
    vector<string> str;
    /*根据user_send，在inter中的state变量查找对应回复的内容*/
    int else_index = -1;
    for (int i = 0; i < inter.state[current_state].size(); i++) {
        /*遍历当前用户会话所在的对话块，顺便找到块中else语句的位置*/
        if (inter.state[current_state][i].accept == "") {
            else_index = i;
        }
        /*找到对应的回复内容所在位置*/
        if (inter.state[current_state][i].accept == user_send) {
            /*如果用户本次输入与上一次相同，则按照DSL程序中的回复顺序依次回复*/
            if (last_from_user == user_send) {
                int condition = inter.state[current_state][i].response.size();
                repeat_of_user++;
                str = inter.state[current_state][i].response[repeat_of_user % condition];
            }
            /*若用户输入与上一次不同，重新给last_from_user与repeat_of_user赋值*/
            else {
                last_from_user.clear();
                last_from_user = user_send;
                repeat_of_user = 0;
                str = inter.state[current_state][i].response[0];
            }
            /*如果用户输入引发了语句块的跳转，则将current_state进行修改*/
            if (inter.state[current_state][i].jump) {
                current_state = inter.state[current_state][i].next_state;
            }
        }
    }
    /*如果遍历完还没有找到对应的回复内容，则跳到current_state内的else块中*/
    if (str.size() == 0 && else_index != -1) {
        int condition = inter.state[current_state][else_index].response.size();
        /*如果本次和上一次均属于else模块，则按照DSL程序中的回复顺序依次回复*/
        if (last_from_user == "" && output.size() != 0) {
            repeat_of_user++;
        }
        /*若初次到达或连续达到else模块，则重新给last_from_user与repeat_of_user赋值*/
        else {
            last_from_user = "";
            repeat_of_user = 0;
        }
        str = inter.state[current_state][else_index].response[repeat_of_user % condition];
        /*如果用户输入引发了语句块的跳转，则将current_state进行修改*/
        if (inter.state[current_state][else_index].jump) {
            current_state = inter.state[current_state][else_index].next_state;
        }
    }
    return str;
}