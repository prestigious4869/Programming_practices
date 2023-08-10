#include "interpreter.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/*����DSL�����еĿո�*/
void interpreter::skip_blank(int& i, string str) {
	for (; str[i] == '\t' || str[i] == ' '; i++);
}

/*��һ��ȫ��Ϊ���ֵ��ַ�������ת��Ϊ����*/
int interpreter::str_to_num(string str) {
	int symbol = 1;
	int num = 0;
	for (int i = 0; i < (int)str.size(); i++) {
		if (i == 0 && str[i] == '-') {
			symbol = -1;
		}
		else {
			num *= 10;
			num += str[i] - '0';
		}
	}
	return (num * symbol);
}

/*ΪIDENTIFIER״̬ʱ����ó��򣬶�ȡһ���ؼ��ֻ��������Ϊtoken*/
void interpreter::ConditionOfIdentifier(int& i, string str) {
	string token = "";
	/*���ӵڶ����ַ���ʼΪ��ĸ�����ֻ��»������Ϊ��ǰtoken*/
	do {
		token += str[i];
		i++;
	} while (isalpha(str[i]) || isdigit(str[i]) || str[i] == '_');

	/*���tokenΪ�ؼ��֣��޸���Ӧ��bool����������������״̬ʱ����Ӧ������ֵ*/
	if (KEYWORD.count(token) == 1) {
		if (token == "robot_name") {
			isRobotName = true;
		}
		else if (token == "window_size") {
			isWindowSize = true;
		}
		else if (token == "begin") {
			isBegin = true;
			isAccept = false;
		}
		else if (token == "jump") {
			state[begin_index][dialog_index].jump = true;
		}
		else if (token == "default") {
			isDefault = true;
		}
	}
	/*token��Ϊ�ؼ��֣���Ϊ����*/
	else {
		/*�������accept��response���֣���Ϊ������ʼ�����֣���Ҫ������variate����*/
		if (!isAccept && !isResponse) {
			if (variate == "") {
				variate = token;
			}
			/*��variate��Ϊ�գ����������Ϊ��variate��ֵ����һ����*/
			else {
				broken_str += str_variate[token];
			}
		}
		/*��Ϊaccept��response���֣���Ϊ���ջ�ش�����һ����*/
		else {
			broken_str += str_variate[token];
		}
	}
}

/*ΪCONSTANT״̬ʱ����ó��򣬶�ȡһ��������Ϊtoken*/
void interpreter::ConditionOfConstant(int& i, string str) {
	string token = "";
	int statement = 0, num = 0;
	/*��ȡ������ǰ����԰�������*/
	do {
		switch (statement) {
		case 0:
			token += str[i];
			if (str[i] == '-') {
				statement = 1;
			}
			else {
				statement = 2;
			}
			break;

		case 1:
			token += str[i];
			statement = 2;
			break;

		case 2:
			token += str[i];
			break;

		default:
			break;
		}
		i++;
	} while (isdigit(str[i]) && i < (int)str.size());

	num = str_to_num(token);
	/*����isWindowSize == true���ʶ�ȡ��������Ϊwindowsize�Ĳ���*/
	if (isWindowSize) {
		if (window_size.first == 0 && window_size.second == 0) {
			window_size.first = num;
		}
		else {
			window_size.second = num;
			isWindowSize = false;
		}
	}
	/*ͬ����ȡ��������ΪDSL�Ի���ı��*/
	else if (isBegin) {
		begin_index = num;
		dialog_index = 0;
		isBegin = false;
		isAccept = true;
	}
	/*��ȡ��������Ϊĳһ�Ի����е���תĿ��*/
	else if (state[begin_index][dialog_index].jump == true) {
		state[begin_index][dialog_index].next_state = num;
	}
}

/*ΪOPERATOR״̬ʱ����ó��򣬶�ȡһ���������Ϊtoken*/
void interpreter::ConditionOfOperator(int& i, string str, int& statement) {
	switch (str[i]) {
	/*��ȡ��"->"��Ҫ�л�״̬��˵��accept��ȡ��ϣ����Ŷ�ȡ��Ϊresponse�����ݣ�����ҪΪresponse���ٿռ�*/
	case '-':
		if (i + 1 < (int)str.size() && str[i + 1] == '>') {
			gram new_dialog;
			vector<string> new_response;
			state[begin_index].push_back(new_dialog);
			state[begin_index][dialog_index].response.push_back(new_response);

			isAccept = false;
			isResponse = true;
			state[begin_index][dialog_index].accept = broken_str;
			broken_str = "";
			i += 2;
		}
		/*���⣬����Ϊһ��������ͷ����Ҫ��ת״̬*/
		else {
			statement = CONSTANT;
		}
		break;
	/*��ȡ��'\"'��˵�����������ַ�������Ҫ�л�״̬*/
	case '\"':
		statement = STRING;
		i++;
		break;
	/*����","��˵��Ϊͬһ���ش�Ĳ���������ֻ��߿�ͷ��Ĭ���������*/
	case ',':
		if (isResponse) {
			state[begin_index][dialog_index].response[string_index].push_back(broken_str);
			broken_str = "";
		}
		else if (isDefault) {
			default_response.push_back(broken_str);
			broken_str = "";
		}
		i++;
		break;

	/*��ȡ��";"��˵��һ���Ի�����������һ��Ի�������ϣ��������ʼ���Ľ���*/
	case ';':
		i++;
		/*�ַ�������response�����ַ���������Ӧλ�õ�response�У����л�response��accept��״̬*/
		if (isResponse) {
			state[begin_index][dialog_index].response[string_index].push_back(broken_str);
			broken_str = "";

			isResponse = false;
			isAccept = true;
			dialog_index++;
			string_index = 0;
		}
		/*�ַ�������default_response�Ľ��*/
		else if (isDefault) {
			default_response.push_back(broken_str);
			broken_str = "";
			isDefault = false;
		}
		/*������ʼ������ô���ַ�����ֵ����Ӧ����*/
		else if (variate != "") {
			str_variate[variate] = broken_str;
			variate = "";
			broken_str = "";
		}
		break;

	default:
		i++;
		break;
	}
}

/*ΪSTRING״̬ʱ����ó��򣬶�ȡһ���ַ�����Ϊtoken*/
void interpreter::ConditionOfString(int& i, string str) {
	string token = "";
	for (; i < (int)str.size() && str[i] != '"'; i++) {
		token += str[i];
	}
	i++;

	/*�����ǰstringΪrobot_name����default_response*/
	if (isRobotName) {
		robot_name = token;
		isRobotName = false;
	}
	else if (isDefault) {
		broken_str += token;
	}
	/*��������£��������ַ��������ӣ����Ѿ�������β��������response�����ַ���������Ӧλ�õ�response�У�����Ϊ��һ��response���ٿռ�*/
	else {
		broken_str += token;
		if (isResponse && i >= (int)str.size()) {
			vector<string> new_response;
			state[begin_index][dialog_index].response.push_back(new_response);
			state[begin_index][dialog_index].response[string_index].push_back(broken_str);
			broken_str = "";
			string_index++;
		}
	}
}

/*ΪANNOTATION״̬ʱ����ó��򣬶�ȡע�Ͳ�����*/
void interpreter::ConditionOfAnnotation(int& i, string& str, ifstream& f) {
	i++;
	/*�ڳ����в���Ѱ����һ��'/'����֮Ϊע�͵Ľ���*/
	while (true) {
		if (i >= (int)str.size()) {
			getline(f, str);
			i = 0;
		}
		for (; i<(int)str.size() && str[i] != '/'; i++);
		if (str[i] == '/') {
			i++;
			break;
		}
	}
}