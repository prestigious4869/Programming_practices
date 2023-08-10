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
	/*读取文件，以每一行为单位分析*/
	while (getline(f, str)) {
		int state = INITIAL;
		int i = 0;
		while (i < (int)str.size()) {
			switch (state) {
			case INITIAL:
				/*跳过空格*/
				skip_blank(i, str);
				/*如果读取到字母或下划线，则进入标识符状态*/
				if (isalpha(str[i]) || str[i] == '_') {
					state = IDENTIFIER;
				}
				/*如果读取到数字，则进入常量状态*/
				else if (isdigit(str[i])) {
					state = CONSTANT;
				}
				/*如果读取到运算符，则进入运算符状态*/
				else if (OPERATE.count(string(1, str[i])) == 1) {
					state = OPERATOR;
				}
				/*如果读取到'/'，则进入注释状态*/
				else if (str[i] == '/') {
					state = ANNOTATION;
				}
				else {
					state = ERROR;
				}
				break;
			/*根据不同状态跳转进入每一种token的分析函数中*/
			case IDENTIFIER:
				ConditionOfIdentifier(i, str);//当token为标识符时
				state = INITIAL;
				break;

			case CONSTANT:
				ConditionOfConstant(i, str); //当token为常量时
				state = INITIAL;
				break;

			case OPERATOR:
				ConditionOfOperator(i, str, state);//运算符
				if (state != STRING && state != CONSTANT)
					state = INITIAL;
				break;

			case ANNOTATION:
				ConditionOfAnnotation(i, str, f);//注释
				state = INITIAL;
				break;

			case STRING:
				ConditionOfString(i, str);//字符串
				state = INITIAL;
				break;

			default:
				printf("ERROR");
			}
		}
	}
	f.close();
}
