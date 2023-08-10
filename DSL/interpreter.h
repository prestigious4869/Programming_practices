#pragma once
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
using namespace std;

/*设置分析DSL程序时自动机所处的各种状态*/
#define ERROR -1
#define INITIAL 0
#define IDENTIFIER 1
#define CONSTANT 2
#define OPERATOR 3
#define ANNOTATION 4
#define STRING 5

/*初始化关键字和运算符*/
static set<string> KEYWORD = { "robot_name","window_size","begin","jump","else","default"};
static set<string> OPERATE = { "+","-","*","\"",">",";","=",":","(",")", "," };

class interpreter
{
public:
	/*存储对话信息的数据结构*/
	typedef struct grammer {
		string accept = "";
		vector<vector<string>> response;
		bool jump = false;
		int next_state = -1;
	}gram;
	string filename;
	unordered_map<int, vector<gram>> state;/*对话块——对话内容*/
	unordered_map<string, string> str_variate;/*变量名——变量内容*/
	pair<int, int> window_size = { 0,0 };//保存脚本程序中定义的窗口尺寸
	vector<string> default_response; //保存脚本程序中定义的初始回复
	string robot_name = "robot";//保存脚本程序中定义的robot_name
	
	interpreter(string file);//构造一个解释器对象，解释file中的DSL程序，并保存在上述数据结构中
	int str_to_num(string);//将一个全部为数字的字符串类型转变为整型
	void skip_blank(int& , string);//跳过DSL程序中的空格
	void ConditionOfIdentifier(int&, string);//为IDENTIFIER状态时进入该程序，读取一个关键字或变量名作为token
	void ConditionOfConstant(int&, string);//为CONSTANT状态时进入该程序，读取一个常量作为token
	void ConditionOfOperator(int&, string, int&);//为OPERATOR状态时进入该程序，读取一个运算符作为token
	void ConditionOfString(int&, string);//为STRING状态时进入该程序，读取一个字符串作为token
	void ConditionOfAnnotation(int&, string&, ifstream&);//为ANNOTATION状态时进入该程序，读取注释并跳过

private:
	/*
		以下为解释DSL程序过程中用到的标识，
		如isRobotName == true
		那么下一个读取的字符串将赋值给robot_name
	*/
	bool isDefault = false;
	bool isRobotName = false;
	bool isWindowSize = false;
	bool isBegin = false;
	bool isAccept = false;
	bool isResponse = false;
	int begin_index = 0;//记录对话块的索引
	int dialog_index = 0;//记录对话的索引
	int string_index = 0;//记录回复的索引
	string broken_str = "";//由于支持字符串之间的连接操作，故需要设置broken_str保存当前读取到的字符串
	string variate = "";//保存当前读取的用户自定义的变量
};