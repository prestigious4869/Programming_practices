#include "DSL.h"
#include <iostream>
#include <QKeyEvent>
#include <sstream>
#include <iomanip>
#include <QTextCodec>
#include <fstream>
#include <windows.h>
using namespace std;

/*DSL���ڵĹ��캯��*/
DSL::DSL(QWidget *parent, string text, string test)
    : QMainWindow(parent),textfile(text),inter(text),testfile(test)
{   
    dsl.setupUi(this);
    /*����DSL�����޸Ĵ��ڴ�С��robot_name*/
    if (inter.window_size.first >= 400 && inter.window_size.second >= 300) {
        this->resize(inter.window_size.first, inter.window_size.second);
    }
    if(inter.robot_name != "")
        robot_name = inter.robot_name;
    /*��ʼ�Ի���Ӧ��ΪDSL�����еĵ�һ���Ի���*/
    if(inter.state.size() != 0)
        current_state = inter.state.begin()->first;

    /*����DSL���ڵ�״̬�����塢���ź�ʹ���û����ͺ����SendFromUser����*/
    dsl.textEdit->setReadOnly(true);
    dsl.textEdit_2->setFont(QFont("songti", 12));
    dsl.textEdit_2->grabKeyboard();
    connect(dsl.pushButton, &QPushButton::clicked, this, &DSL::SendFromUser);

    /*DSL�������robot��Ĭ�ϻظ�����ôӦ�����ȵ���SendFromRobot*/
    for (int i = 0; i < inter.default_response.size(); i++) {
        SendFromRobot(inter.default_response[i]);
    }

    /*���testfile��Ϊ�գ�˵���û����õ����Զ������Է�ʽ����Ҫ���ļ������������*/
    if (testfile != "") {
        ifstream f;
        f.open(testfile, ios::in);
        while (getline(f, user_send)) {
            SendFromUser();
        }
        f.close();
    }     
}

/*���û�����ctrl+enterʱ���ͶԻ�������SendFromUser����*/
void DSL::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) {
        SendFromUser();
    }
}

/*����û���DSL�����е����룬������ʾ�ڶԻ����У�������SendFromRobot*/
void DSL::SendFromUser() {
    QString str;
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    /*testfile��Ϊ�գ���Ϊ�Զ������ԣ�ֱ�Ӵ�user_send��ȡ������*/
    if (testfile != "") {
        str = pCodec->toUnicode(user_send.c_str(), user_send.length());
    }
    /*��Ϊ�Զ������ԣ����û�������ȡ����*/
    else {
        str = dsl.textEdit_2->toPlainText();
        user_send = pCodec->fromUnicode(str).data();
    }

    /*��ȡϵͳʱ��*/
    auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream ss;
    ss << put_time(localtime(&t), "%Y/%m/%d-%H:%M:%S");
    QString time = QString::fromStdString(ss.str());


    /*��ӡʱ����Ϣ�������䱣���ڶԻ����Ҳ�*/
    QTextCharFormat fmt;
    fmt.setForeground(QColor("blue"));
    fmt.setFont(QFont("kaiti", 10));
    QTextCursor cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(time);
    dsl.textEdit->setAlignment(Qt::AlignRight);

    /*��ӡ�Ի����ݣ������䱣���ڶԻ����Ҳ�*/
    fmt.setForeground(QColor("black"));
    fmt.setFont(QFont("songti", 15));
    cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(str);
    dsl.textEdit->setAlignment(Qt::AlignRight);

    /*����걣�����û��������*/
    cursor = dsl.textEdit_2->textCursor();
    dsl.textEdit_2->clear();
    dsl.textEdit_2->setFocus();

    /*����Replystring������ȡ�������*/
    vector<string> reply = ReplyString();
    for (int i = 0; i < reply.size(); i++) {
        if(reply[i]!="")
            SendFromRobot(reply[i]);
    }
}

/*����ReplyString�������ظ�������ʾ�ڶԻ�����*/
void DSL::SendFromRobot(string str) {
    output.push_back(str);

    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    QString qstr = pCodec->toUnicode(str.c_str(), str.length());

    /*��ȡϵͳʱ��*/
    auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
    stringstream ss;
    ss << put_time(localtime(&t), "%Y/%m/%d-%H:%M:%S");
    QString time = pCodec->toUnicode((ss.str() + " - " + this->robot_name).c_str(), (ss.str() + " - " + this->robot_name).length());

    /*��ӡʱ����Ϣ�������䱣���ڶԻ������*/
    QTextCharFormat fmt;
    fmt.setForeground(QColor("blue"));
    fmt.setFont(QFont("kaiti", 10));
    QTextCursor cursor = dsl.textEdit->textCursor();
    cursor.mergeBlockCharFormat(fmt);
    dsl.textEdit->mergeCurrentCharFormat(fmt);
    dsl.textEdit->append(time);
    dsl.textEdit->setAlignment(Qt::AlignLeft);

    /*��ӡ�Ի����ݣ������䱣���ڶԻ������*/
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
    /*����user_send����inter�е�state�������Ҷ�Ӧ�ظ�������*/
    int else_index = -1;
    for (int i = 0; i < inter.state[current_state].size(); i++) {
        /*������ǰ�û��Ự���ڵĶԻ��飬˳���ҵ�����else����λ��*/
        if (inter.state[current_state][i].accept == "") {
            else_index = i;
        }
        /*�ҵ���Ӧ�Ļظ���������λ��*/
        if (inter.state[current_state][i].accept == user_send) {
            /*����û�������������һ����ͬ������DSL�����еĻظ�˳�����λظ�*/
            if (last_from_user == user_send) {
                int condition = inter.state[current_state][i].response.size();
                repeat_of_user++;
                str = inter.state[current_state][i].response[repeat_of_user % condition];
            }
            /*���û���������һ�β�ͬ�����¸�last_from_user��repeat_of_user��ֵ*/
            else {
                last_from_user.clear();
                last_from_user = user_send;
                repeat_of_user = 0;
                str = inter.state[current_state][i].response[0];
            }
            /*����û������������������ת����current_state�����޸�*/
            if (inter.state[current_state][i].jump) {
                current_state = inter.state[current_state][i].next_state;
            }
        }
    }
    /*��������껹û���ҵ���Ӧ�Ļظ����ݣ�������current_state�ڵ�else����*/
    if (str.size() == 0 && else_index != -1) {
        int condition = inter.state[current_state][else_index].response.size();
        /*������κ���һ�ξ�����elseģ�飬����DSL�����еĻظ�˳�����λظ�*/
        if (last_from_user == "" && output.size() != 0) {
            repeat_of_user++;
        }
        /*�����ε���������ﵽelseģ�飬�����¸�last_from_user��repeat_of_user��ֵ*/
        else {
            last_from_user = "";
            repeat_of_user = 0;
        }
        str = inter.state[current_state][else_index].response[repeat_of_user % condition];
        /*����û������������������ת����current_state�����޸�*/
        if (inter.state[current_state][else_index].jump) {
            current_state = inter.state[current_state][else_index].next_state;
        }
    }
    return str;
}