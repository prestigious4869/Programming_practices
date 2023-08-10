#include "Request.h"
#include <fstream>
#include <sstream>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    /*创建Request窗口接收用户的需求方式*/
    QApplication a(argc, argv);
    Request file;
    file.show();
    a.exec();

    /*若成功在Request内创建DSL窗口，则进行输出结果判断*/
    if (file.dialog != NULL) {
        ifstream result;
        ofstream out;
        /****正确输出的文件名应改为测试数据文件首字母+"result.out"****/
        string resultfile = string(1, file.dialog->testfile[0]) + "result.out";
        result.open(resultfile, ios::in);
        out.open("output.out", ios::trunc | ios::out);
        vector<int> error_id;//保存错误行数
        bool correct = true;
        int i = 0;
        string str = "";
        /*对比测试结果与正确结果文件的每一行*/
        for (; i < (int)file.dialog->output.size(); i++) {
            getline(result, str);
            out << file.dialog->output[i] << '\n';
            if (file.dialog->output[i] != str) {
                error_id.push_back(i+1);
                correct = false;
            }
        }
        /*如果测试文件读取完毕依然为correct，且正确结果文件读取完毕，则输出correct*/
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
        /*释放创建的DSL窗口内存*/
        delete file.dialog;
    }
}
