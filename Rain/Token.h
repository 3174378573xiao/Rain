#pragma once
#include<string>
#include <variant>
#include <map>
using namespace std;
enum TokenType {
	// 单字符标记
	LEFT_PAREN,     // (   左小括号
	RIGHT_PAREN,    // )   右小括号
	LEFT_BRACE,     // {   左大括号
	RIGHT_BRACE,    // }   右大括号
	COMMA,          // ,   逗号
	DOT,            // .   点号
	MINUS,          // -   减号/负号
	PLUS,           // +   加号/正号
	SEMICOLON,      // ;   分号
	SLASH,          // /   斜杠(除法或注释)
	STAR,           // *   星号(乘法)

	// 一或两个字符的标记
	BANG,           // !   感叹号(逻辑非)
	BANG_EQUAL,     // !=  不等号
	EQUAL,          // =   等号(赋值)
	EQUAL_EQUAL,    // ==  相等比较
	GREATER,        // >   大于号
	GREATER_EQUAL,  // >=  大于等于
	LESS,           // <   小于号
	LESS_EQUAL,     // <=  小于等于

	// 字面量
	IDENTIFIER,     //     标识符(变量名/函数名等)
	STRING,         //     字符串字面量
	NUMBER,         //     数字字面量

	// 关键字
	AND,            // and     逻辑与
	CLASS,          // class   类定义
	ELSE,           // else    if语句的else分支
	FALSE,          // false   布尔假值
	FUN,            // fun     函数定义
	FOR,            // for     for循环
	IF,             // if      if条件语句
	NIL,            // nil     空值
	OR,             // or      逻辑或
	PRINT,          // print   打印语句
	RETURN,         // return  函数返回
	SUPER,          // super   父类引用
	THIS,           // this    当前对象引用
	TRUE,           // true    布尔真值
	VAR,            // var     变量声明
	WHILE,          // while   while循环

	END             //         源代码结束标记
};

map<TokenType, string> trans;

//rain语言 词法分析后代码转换为Token流
//作者：xiao
//github:3174378573xiao
//qq:3174378573
class Token
{
public:
	TokenType type;
	string lexeme;//词素 有双括号
#define object variant<double , string ,int>
	object literal;//原本意思
	int line;
	Token(TokenType type, string lexeme, object literal, int line)
		: type(type), lexeme(lexeme), literal(literal), line(line) {
			{
				// 单字符标记
				trans[LEFT_PAREN] = "LEFT_PAREN";
				trans[RIGHT_PAREN] = "RIGHT_PAREN";
				trans[LEFT_BRACE] = "LEFT_BRACE";
				trans[RIGHT_BRACE] = "RIGHT_BRACE";
				trans[COMMA] = "COMMA";
				trans[DOT] = "DOT";
				trans[MINUS] = "MINUS";
				trans[PLUS] = "PLUS";
				trans[SEMICOLON] = "SEMICOLON";
				trans[SLASH] = "SLASH";
				trans[STAR] = "STAR";
				//一或两个字符的标记
				trans[BANG] = "BANG";
				trans[BANG_EQUAL] = "BANG_EQUAL";
				trans[EQUAL] = "EQUAL";
				trans[EQUAL_EQUAL] = "EQUAL_EQUAL";
				trans[GREATER] = "GREATER";
				trans[GREATER_EQUAL] = "GREATER_EQUAL";
				trans[LESS] = "LESS";
				trans[LESS_EQUAL] = "LESS_EQUAL";
				//字面量
				trans[IDENTIFIER] = "IDENTIFIER";
				trans[STRING] = "STRING";
				trans[NUMBER] = "NUMBER";
				//关键字
				trans[AND] = "AND";
				trans[CLASS] = "CLASS";
				trans[ELSE] = "ELSE";
				trans[FALSE] = "FALSE";
				trans[FUN] = "FUN";
				trans[FOR] = "FOR";
				trans[IF] = "IF";
				trans[NIL] = "NIL";
				trans[OR] = "OR";
				trans[PRINT] = "PRINT";
				trans[RETURN] = "RETURN";
				trans[SUPER] = "SUPER";
				trans[THIS] = "THIS";
				trans[TRUE] = "TRUE";
				trans[VAR] = "VAR";
				trans[WHILE] = "WHILE";
				trans[END] = "END";
			}
	}

	string toString() {
		string literalString;
		if (auto ptr = std::get_if<double>(&literal)) {
			literalString = to_string(*ptr);
			// 如果是整数，去掉小数部分显示
			if (literalString.find('.') != string::npos) {
				// 去除多余的0
				literalString.erase(literalString.find_last_not_of('0') + 1, string::npos);
				if (literalString.back() == '.') {
					literalString.pop_back();
				}
			}
		}
		else if (auto ptr = std::get_if<std::string>(&literal)) {
			literalString = *ptr;
		}
		else if (auto ptr = std::get_if<int>(&literal)) {
			literalString = "NULL";
		}

		// 使用固定宽度格式化输出
		char buffer[256];
		snprintf(buffer, sizeof(buffer), "%-20s %-30s %-20s %-20s",
			trans[type].c_str(),
			lexeme.c_str(),
			literalString.c_str(),
			string("at " + to_string(line) + " line").c_str()
		);

		return buffer;
	}

};