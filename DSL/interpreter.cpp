#include "interpreter.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

interpreter::interpreter(string file)
	:filename(file)
{
	string str;
	ifstream f;
	f.open(this->filename, ios::in);
	/*��ȡ�ļ�����ÿһ��Ϊ��λ����*/
	while (getline(f, str)) {
		int state = INITIAL;
		int i = 0;
		while (i < (int)str.size()) {
			switch (state) {
			case INITIAL:
				/*�����ո�*/
				skip_blank(i, str);
				/*�����ȡ����ĸ���»��ߣ�������ʶ��״̬*/
				if (isalpha(str[i]) || str[i] == '_') {
					state = IDENTIFIER;
				}
				/*�����ȡ�����֣�����볣��״̬*/
				else if (isdigit(str[i])) {
					state = CONSTANT;
				}
				/*�����ȡ�������������������״̬*/
				else if (OPERATE.count(string(1, str[i])) == 1) {
					state = OPERATOR;
				}
				/*�����ȡ��'/'�������ע��״̬*/
				else if (str[i] == '/') {
					state = ANNOTATION;
				}
				else {
					state = ERROR;
				}
				break;
			/*���ݲ�ͬ״̬��ת����ÿһ��token�ķ���������*/
			case IDENTIFIER:
				ConditionOfIdentifier(i, str);//��tokenΪ��ʶ��ʱ
				state = INITIAL;
				break;

			case CONSTANT:
				ConditionOfConstant(i, str); //��tokenΪ����ʱ
				state = INITIAL;
				break;

			case OPERATOR:
				ConditionOfOperator(i, str, state);//�����
				if (state != STRING && state != CONSTANT)
					state = INITIAL;
				break;

			case ANNOTATION:
				ConditionOfAnnotation(i, str, f);//ע��
				state = INITIAL;
				break;

			case STRING:
				ConditionOfString(i, str);//�ַ���
				state = INITIAL;
				break;

			default:
				printf("ERROR");
			}
		}
	}
	f.close();
}
