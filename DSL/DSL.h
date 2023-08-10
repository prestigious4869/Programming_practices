#pragma once
#pragma execution_character_set("UTF-8")
#include <QtWidgets/QMainWindow>
#include "ui_DSL.h"
#include "interpreter.h"
#include <QString>
#include <QKeyEvent>
#include <QCloseEvent>
using namespace std;

class DSL : public QMainWindow
{
    Q_OBJECT

public:
    DSL(QWidget* parent, string text, string test);
    string robot_name;//对话机器人的名字
    string user_send;//用户在DSL窗口中的输入
    string textfile;//需要解释的DSL程序文本名
    string testfile;//自动化测试所需的输入文本名

    void SendFromUser();//获得用户在DSL窗口中的输入，将它显示在对话框中，并调用SendFromRobot
    void SendFromRobot(string);//调用ReplyString，并将回复显示在对话框中
    vector<string> ReplyString();//根据user_send分析回复内容，返回回复语句

    vector<string> output;//记录robot在整个对话过程中回复的所有内容

protected:
    virtual void keyPressEvent(QKeyEvent* event);//位用户输入框绑定快捷键

private:

    interpreter inter;//创建一个interpreter对象，分析DSL程序
    Ui::DSLClass dsl;
    string last_from_user = "";//记录用户上一次输入的内容
    int repeat_of_user = 0;//记录用户重复同一句话的次数
    int current_state;//记录当前对话处于DSL程序中的哪一个对话块
};