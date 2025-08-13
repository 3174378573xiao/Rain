#pragma once
#include<string>
using namespace std;
enum TokenType {
	// 单字符标记
	LEFT_PAREN,		//	(	左小括号
	RIGHT_PAREN,	//	）
	LEFT_BRACE,		//	{
	RIGHT_BRACE,	//	}
	COMMA,			//	,
	DOT,			//	.
	MINUS,			//	-
	PLUS,			//	+
	SEMICOLON,		//	;
	SLASH,			//  /
	STAR,			//	*

	//一或两个字符的标记
	BANG,			//	!
	BANG_EQUAL,		//	!=
	EQUAL,			//	=
	EQUAL_EQUAL,	//	==
	GREATER,		//	>
	GREATER_EQUAL,	//	>=
	LESS,			//	<
	LESS_EQUAL,		//	<=

	//字面量
	IDENTIFIER,		//	标识符
	STRING,			//	string
	NUMBER,			//	number

	//关键字
	AND, 
	CLASS,
	ELSE,
	FALSE,
	FUN,
	FOR,
	IF,
	NIL,
	OR,
	PRINT,
	RETURN,
	SUPER,
	THIS,
	TRUE,
	VAR,
	WHILE,
	END				// 代码结束 EOF
};

class Token
{
public:
	TokenType type;
	string lexeme;
	int line;
	Token(TokenType type, string lexeme, int line) :type(type), lexeme(lexeme), line(line) {

	}

};