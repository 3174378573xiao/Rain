#pragma once
#include<string>
#include<vector>
#include<iostream>
#include"Token.h"
using namespace std;

class Scanner
{
public:

	string source;
	vector<Token> tokens;


	Scanner(string source) :source(source) {
		
	}
private:
	//token开始位置
	int start = 0;
	//当前位置
	int current = 0;
	//所在行数
	int line = 1;

	bool isAtEnd() {
		return current >= source.length();
	}

	void addToken(TokenType type) {
		string lexeme = source.substr(start, current);
		tokens.push_back(Token(type, lexeme, line));
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
		tokens.push_back(Token(END,"",line));
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

	void scanToken() {
		char c = advance();
		switch (c) {

		//单字符符号匹配
		case '(': addToken(LEFT_PAREN); break;
		case ')': addToken(RIGHT_PAREN); break;
		case '{': addToken(LEFT_BRACE); break;
		case '}': addToken(RIGHT_BRACE); break;
		case ',': addToken(COMMA); break;
		case '.': addToken(DOT); break;
		case '-': addToken(MINUS); break;
		case '+': addToken(PLUS); break;
		case ';': addToken(SEMICOLON); break;
		case '*': addToken(STAR); break;


		//单或双字符符号匹配
		case '!':
			addToken(match('=') ? BANG_EQUAL : BANG);
			break;
		case '=':
			addToken(match('=') ? EQUAL_EQUAL : EQUAL);
			break;
		case '<':
			addToken(match('=') ? LESS_EQUAL : LESS);
			break;
		case '>':
			addToken(match('=') ? GREATER_EQUAL : GREATER);
			break;

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

		case ' ':
		case '\r':
		case '\t':
			// 暂时忽略制表符
			break;

			//换行符，增加行数
		case '\n':
			line++;
			break;


		default:
			cout << "[错误]： 在第" << line << "行，不被期望的字符" << endl;;
			break;
		}
	}
};
