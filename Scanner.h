//rain语言 编译中用于词法分析的类
//作者：xiao
//github:3174378573xiao
//qq:3174378573
#pragma once
#include<string>
#include<vector>
#include<map>
#include<iostream>
#include"Token.h"
using namespace std;

class Scanner
{
public:

	string source;
	vector<Token> tokens;
	map<string, TokenType> keywords;


	Scanner(string source) :source(source) {
		//加载关键字
		loadKeywords();
		scanTokens();
		for (int i = 0; i < tokens.size(); i++) {
			cout << i << "\t|" << tokens[i].toString() << endl;
		}
		cout << start << endl;
		cout << current << endl;
	}
private:
	void loadKeywords() {
		keywords["and"] = AND;
		keywords["class"] = CLASS;
		keywords["else"] = ELSE;
		keywords["false"] = FALSE;
		keywords["for"] = FOR;
		keywords["fun"] = FUN;
		keywords["if"] = IF;
		keywords["nil"] = NIL;
		keywords["or"] = OR;
		keywords["print"] = PRINT;
		keywords["return"] = RETURN;
		keywords["super"] = SUPER;
		keywords["this"] = THIS;
		keywords["true"] = TRUE;
		keywords["var"] = VAR;
		keywords["while"] = WHILE;
	}
	//token开始位置
	int start = 0;
	//当前位置
	int current = 0;
	//所在行数
	int line = 1;

	bool isAtEnd() {
		return current >= source.length();
	}

	void addToken(TokenType type, object literal) {
		string lexeme = source.substr(start, current - start);
		tokens.push_back(Token(type, lexeme, literal, line));
	}

	void addToken(TokenType type) {
		addToken(type, NULL);
	}

	//向前推进
	char advance() {
		return source.at(current++);
	}

	void scanTokens() {
		tokens.clear();
		while (!isAtEnd()) {
			start = current;
			scanToken();
		}
		tokens.push_back(Token(END, "", NULL, line));
	}

	//判断下一个字符是否符合预期，并向前推进
	bool match(char expected) {
		if (isAtEnd()) return false;
		if (source.at(current) != expected) return false;

		current++;
		return true;
	}

	//前瞻 返回下一个字符，但不向前推进
	char peek() {
		if (isAtEnd()) return '\0';
		return source.at(current);
	}
	//前瞻两位
	char peekNext() {
		if (current + 1 >= source.length()) return '\0';
		return source.at(current + 1);
	}


	bool isDigit(char c) {
		return c >= '0' && c <= '9';
	}

	bool isAlphaSubline(char c) {
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
	}

	bool isAlphaSublineNumeric(char c) {
		return isAlphaSubline(c) || isDigit(c);
	}

	void scanString() {
		while (peek() != '\"' && !isAtEnd()) {
			if (peek() == '\n') line++;
			advance();
		}

		if (isAtEnd()) {
			cout << "[错误]： 在第" << line << "行，未闭合的字符串字面量" << endl;
			return;
		}

		// 跳过后面的 "
		advance();

		//去掉两个引号				//开始位置	//长度
		string value = source.substr(start + 1, current - start - 2);
		addToken(STRING, value);
	}

	void scanNumber() {
		while (isDigit(peek())) advance();

		//寻找小数部分
		if (peek() == '.' && isDigit(peekNext())) {
			// Consume the "."
			advance();

			while (isDigit(peek())) advance();
		}

		addToken(NUMBER,
			stod(source.substr(start, current - start)));
	}
	void scanIdentifier() {
		while ( isAlphaSublineNumeric( peek() ) ) advance();

		string text = source.substr(start, current - start);
		TokenType type;
		if (keywords.find(text) != keywords.end()) {
			//键存在
			type = keywords[text];
		}
		else {
			//键不存在
			type = IDENTIFIER;
		}
		addToken(type);
	}

	void scanToken() {
		char c = advance();
		switch (c) {

			//单字符符号匹配
		case '(': addToken(LEFT_PAREN);		break;
		case ')': addToken(RIGHT_PAREN);	break;
		case '{': addToken(LEFT_BRACE);		break;
		case '}': addToken(RIGHT_BRACE);	break;
		case ',': addToken(COMMA);			break;
		case '.': addToken(DOT);			break;
		case '-': addToken(MINUS);			break;
		case '+': addToken(PLUS);			break;
		case ';': addToken(SEMICOLON);		break;
		case '*': addToken(STAR);			break;


			//单或双字符符号匹配
		case '!':addToken(match('=') ? BANG_EQUAL : BANG);		break;
		case '=':addToken(match('=') ? EQUAL_EQUAL : EQUAL);	break;
		case '<':addToken(match('=') ? LESS_EQUAL : LESS);		break;
		case '>':addToken(match('=') ? GREATER_EQUAL : GREATER); break;

			//较长词位lexeme
				//单斜杠开头有两种情况
		case '/':
			if (match('/')) {
				//匹配到双斜杠，为注释，跳过当前行
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				//匹配到单斜杠，除法
				addToken(SLASH);
			}
			break;

		case ' ' :
		case '\0'://
		case '\r':
		case '\t':
			// 忽略空白符
			break;

			//换行符，增加行数
		case '\n':
			line++;
			break;

			// " 匹配到引号，为字符串字面量
		case '\"':scanString(); break;

		default:
			if (isDigit(c)) {
				scanNumber();
			}
			else if (isAlphaSubline(c)) {
				scanIdentifier();
			}
			else {
				cout << "[错误]： 在第" << line << "行，不被期望的字符" << endl;
			}
			break;
		}
	}
};
