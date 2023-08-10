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
    string robot_name;//�Ի������˵�����
    string user_send;//�û���DSL�����е�����
    string textfile;//��Ҫ���͵�DSL�����ı���
    string testfile;//�Զ�����������������ı���

    void SendFromUser();//����û���DSL�����е����룬������ʾ�ڶԻ����У�������SendFromRobot
    void SendFromRobot(string);//����ReplyString�������ظ���ʾ�ڶԻ�����
    vector<string> ReplyString();//����user_send�����ظ����ݣ����ػظ����

    vector<string> output;//��¼robot�������Ի������лظ�����������

protected:
    virtual void keyPressEvent(QKeyEvent* event);//λ�û������󶨿�ݼ�

private:

    interpreter inter;//����һ��interpreter���󣬷���DSL����
    Ui::DSLClass dsl;
    string last_from_user = "";//��¼�û���һ�����������
    int repeat_of_user = 0;//��¼�û��ظ�ͬһ�仰�Ĵ���
    int current_state;//��¼��ǰ�Ի�����DSL�����е���һ���Ի���
};