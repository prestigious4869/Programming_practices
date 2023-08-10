#include "Request.h"
#include <fstream>
#include <sstream>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    /*����Request���ڽ����û�������ʽ*/
    QApplication a(argc, argv);
    Request file;
    file.show();
    a.exec();

    /*���ɹ���Request�ڴ���DSL���ڣ�������������ж�*/
    if (file.dialog != NULL) {
        ifstream result;
        ofstream out;
        /****��ȷ������ļ���Ӧ��Ϊ���������ļ�����ĸ+"result.out"****/
        string resultfile = string(1, file.dialog->testfile[0]) + "result.out";
        result.open(resultfile, ios::in);
        out.open("output.out", ios::trunc | ios::out);
        vector<int> error_id;//�����������
        bool correct = true;
        int i = 0;
        string str = "";
        /*�ԱȲ��Խ������ȷ����ļ���ÿһ��*/
        for (; i < (int)file.dialog->output.size(); i++) {
            getline(result, str);
            out << file.dialog->output[i] << '\n';
            if (file.dialog->output[i] != str) {
                error_id.push_back(i+1);
                correct = false;
            }
        }
        /*��������ļ���ȡ�����ȻΪcorrect������ȷ����ļ���ȡ��ϣ������correct*/
        if (result.eof() && correct) {
            out << "correct";
        }
        else {
            out << "incorrect" << "\n" << "The ";
            for (int x = 0; x < error_id.size(); x++) {
                out << " " << error_id[x] << "st";
            }
            out<< " line has error!";
        }
        result.close();
        out.close();
        /*�ͷŴ�����DSL�����ڴ�*/
        delete file.dialog;
    }
}
