//rain���� ���������ڴʷ���������
//���ߣ�xiao
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
		//���عؼ���
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
	//token��ʼλ��
	int start = 0;
	//��ǰλ��
	int current = 0;
	//��������
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

	//��ǰ�ƽ�
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

	//�ж���һ���ַ��Ƿ����Ԥ�ڣ�����ǰ�ƽ�
	bool match(char expected) {
		if (isAtEnd()) return false;
		if (source.at(current) != expected) return false;

		current++;
		return true;
	}

	//ǰհ ������һ���ַ���������ǰ�ƽ�
	char peek() {
		if (isAtEnd()) return '\0';
		return source.at(current);
	}
	//ǰհ��λ
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
			cout << "[����]�� �ڵ�" << line << "�У�δ�պϵ��ַ���������" << endl;
			return;
		}

		// ��������� "
		advance();

		//ȥ����������				//��ʼλ��	//����
		string value = source.substr(start + 1, current - start - 2);
		addToken(STRING, value);
	}

	void scanNumber() {
		while (isDigit(peek())) advance();

		//Ѱ��С������
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
			//������
			type = keywords[text];
		}
		else {
			//��������
			type = IDENTIFIER;
		}
		addToken(type);
	}

	void scanToken() {
		char c = advance();
		switch (c) {

			//���ַ�����ƥ��
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


			//����˫�ַ�����ƥ��
		case '!':addToken(match('=') ? BANG_EQUAL : BANG);		break;
		case '=':addToken(match('=') ? EQUAL_EQUAL : EQUAL);	break;
		case '<':addToken(match('=') ? LESS_EQUAL : LESS);		break;
		case '>':addToken(match('=') ? GREATER_EQUAL : GREATER); break;

			//�ϳ���λlexeme
				//��б�ܿ�ͷ���������
		case '/':
			if (match('/')) {
				//ƥ�䵽˫б�ܣ�Ϊע�ͣ�������ǰ��
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else {
				//ƥ�䵽��б�ܣ�����
				addToken(SLASH);
			}
			break;

		case ' ' :
		case '\0'://
		case '\r':
		case '\t':
			// ���Կհ׷�
			break;

			//���з�����������
		case '\n':
			line++;
			break;

			// " ƥ�䵽���ţ�Ϊ�ַ���������
		case '\"':scanString(); break;

		default:
			if (isDigit(c)) {
				scanNumber();
			}
			else if (isAlphaSubline(c)) {
				scanIdentifier();
			}
			else {
				cout << "[����]�� �ڵ�" << line << "�У������������ַ�" << endl;
			}
			break;
		}
	}
};
