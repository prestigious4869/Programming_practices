#pragma once
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
using namespace std;

/*���÷���DSL����ʱ�Զ��������ĸ���״̬*/
#define ERROR -1
#define INITIAL 0
#define IDENTIFIER 1
#define CONSTANT 2
#define OPERATOR 3
#define ANNOTATION 4
#define STRING 5

/*��ʼ���ؼ��ֺ������*/
static set<string> KEYWORD = { "robot_name","window_size","begin","jump","else","default"};
static set<string> OPERATE = { "+","-","*","\"",">",";","=",":","(",")", "," };

class interpreter
{
public:
	/*�洢�Ի���Ϣ�����ݽṹ*/
	typedef struct grammer {
		string accept = "";
		vector<vector<string>> response;
		bool jump = false;
		int next_state = -1;
	}gram;
	string filename;
	unordered_map<int, vector<gram>> state;/*�Ի��顪���Ի�����*/
	unordered_map<string, string> str_variate;/*������������������*/
	pair<int, int> window_size = { 0,0 };//����ű������ж���Ĵ��ڳߴ�
	vector<string> default_response; //����ű������ж���ĳ�ʼ�ظ�
	string robot_name = "robot";//����ű������ж����robot_name
	
	interpreter(string file);//����һ�����������󣬽���file�е�DSL���򣬲��������������ݽṹ��
	int str_to_num(string);//��һ��ȫ��Ϊ���ֵ��ַ�������ת��Ϊ����
	void skip_blank(int& , string);//����DSL�����еĿո�
	void ConditionOfIdentifier(int&, string);//ΪIDENTIFIER״̬ʱ����ó��򣬶�ȡһ���ؼ��ֻ��������Ϊtoken
	void ConditionOfConstant(int&, string);//ΪCONSTANT״̬ʱ����ó��򣬶�ȡһ��������Ϊtoken
	void ConditionOfOperator(int&, string, int&);//ΪOPERATOR״̬ʱ����ó��򣬶�ȡһ���������Ϊtoken
	void ConditionOfString(int&, string);//ΪSTRING״̬ʱ����ó��򣬶�ȡһ���ַ�����Ϊtoken
	void ConditionOfAnnotation(int&, string&, ifstream&);//ΪANNOTATION״̬ʱ����ó��򣬶�ȡע�Ͳ�����

private:
	/*
		����Ϊ����DSL����������õ��ı�ʶ��
		��isRobotName == true
		��ô��һ����ȡ���ַ�������ֵ��robot_name
	*/
	bool isDefault = false;
	bool isRobotName = false;
	bool isWindowSize = false;
	bool isBegin = false;
	bool isAccept = false;
	bool isResponse = false;
	int begin_index = 0;//��¼�Ի��������
	int dialog_index = 0;//��¼�Ի�������
	int string_index = 0;//��¼�ظ�������
	string broken_str = "";//����֧���ַ���֮������Ӳ���������Ҫ����broken_str���浱ǰ��ȡ�����ַ���
	string variate = "";//���浱ǰ��ȡ���û��Զ���ı���
};