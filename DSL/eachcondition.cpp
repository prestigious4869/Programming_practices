#include "interpreter.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

/*跳过DSL程序中的空格*/
void interpreter::skip_blank(int& i, string str) {
	for (; str[i] == '\t' || str[i] == ' '; i++);
}

/*将一个全部为数字的字符串类型转变为整型*/
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

/*为IDENTIFIER状态时进入该程序，读取一个关键字或变量名作为token*/
void interpreter::ConditionOfIdentifier(int& i, string str) {
	string token = "";
	/*若从第二个字符开始为字母、数字或下划线则均为当前token*/
	do {
		token += str[i];
		i++;
	} while (isalpha(str[i]) || isdigit(str[i]) || str[i] == '_');

	/*如果token为关键字，修改相应的bool变量，方便在其他状态时对相应变量赋值*/
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
	/*token不为关键字，即为变量*/
	else {
		/*如果不在accept和response部分，则为变量初始化部分，需要单独对variate操作*/
		if (!isAccept && !isResponse) {
			if (variate == "") {
				variate = token;
			}
			/*若variate不为空，则这个变量为对variate赋值语句的一部分*/
			else {
				broken_str += str_variate[token];
			}
		}
		/*若为accept或response部分，则为接收或回答语句的一部分*/
		else {
			broken_str += str_variate[token];
		}
	}
}

/*为CONSTANT状态时进入该程序，读取一个常量作为token*/
void interpreter::ConditionOfConstant(int& i, string str) {
	string token = "";
	int statement = 0, num = 0;
	/*读取整数，前面可以包括负号*/
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
	/*由于isWindowSize == true，故读取到的数字为windowsize的参数*/
	if (isWindowSize) {
		if (window_size.first == 0 && window_size.second == 0) {
			window_size.first = num;
		}
		else {
			window_size.second = num;
			isWindowSize = false;
		}
	}
	/*同理，读取到的数字为DSL对话块的编号*/
	else if (isBegin) {
		begin_index = num;
		dialog_index = 0;
		isBegin = false;
		isAccept = true;
	}
	/*读取到的数字为某一对话块中的跳转目标*/
	else if (state[begin_index][dialog_index].jump == true) {
		state[begin_index][dialog_index].next_state = num;
	}
}

/*为OPERATOR状态时进入该程序，读取一个运算符作为token*/
void interpreter::ConditionOfOperator(int& i, string str, int& statement) {
	switch (str[i]) {
	/*读取到"->"需要切换状态，说明accept读取完毕，接着读取的为response的内容，故需要为response开辟空间*/
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
		/*此外，可能为一个负数开头，需要跳转状态*/
		else {
			statement = CONSTANT;
		}
		break;
	/*读取到'\"'，说明接下来是字符串，需要切换状态*/
	case '\"':
		statement = STRING;
		i++;
		break;
	/*读到","，说明为同一级回答的并列输出部分或者开头的默认输出部分*/
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

	/*读取到";"，说明一个对话块中完整的一组对话分析完毕，或变量初始化的结束*/
	case ';':
		i++;
		/*字符串属于response，将字符串加入相应位置的response中，并切换response和accept的状态*/
		if (isResponse) {
			state[begin_index][dialog_index].response[string_index].push_back(broken_str);
			broken_str = "";

			isResponse = false;
			isAccept = true;
			dialog_index++;
			string_index = 0;
		}
		/*字符串属于default_response的结果*/
		else if (isDefault) {
			default_response.push_back(broken_str);
			broken_str = "";
			isDefault = false;
		}
		/*变量初始化，那么将字符串赋值给对应变量*/
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

/*为STRING状态时进入该程序，读取一个字符串作为token*/
void interpreter::ConditionOfString(int& i, string str) {
	string token = "";
	for (; i < (int)str.size() && str[i] != '"'; i++) {
		token += str[i];
	}
	i++;

	/*如果当前string为robot_name或是default_response*/
	if (isRobotName) {
		robot_name = token;
		isRobotName = false;
	}
	else if (isDefault) {
		broken_str += token;
	}
	/*其他情况下，首先做字符串的连接，若已经到达行尾，且属于response，则将字符串加入相应位置的response中，接着为下一个response开辟空间*/
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

/*为ANNOTATION状态时进入该程序，读取注释并跳过*/
void interpreter::ConditionOfAnnotation(int& i, string& str, ifstream& f) {
	i++;
	/*在程序中不断寻找下一个'/'，以之为注释的结束*/
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