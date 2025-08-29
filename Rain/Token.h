#pragma once
#include<string>
#include <variant>
#include <map>
using namespace std;
enum TokenType {
	// ���ַ����
	LEFT_PAREN,     // (   ��С����
	RIGHT_PAREN,    // )   ��С����
	LEFT_BRACE,     // {   �������
	RIGHT_BRACE,    // }   �Ҵ�����
	COMMA,          // ,   ����
	DOT,            // .   ���
	MINUS,          // -   ����/����
	PLUS,           // +   �Ӻ�/����
	SEMICOLON,      // ;   �ֺ�
	SLASH,          // /   б��(������ע��)
	STAR,           // *   �Ǻ�(�˷�)

	// һ�������ַ��ı��
	BANG,           // !   ��̾��(�߼���)
	BANG_EQUAL,     // !=  ���Ⱥ�
	EQUAL,          // =   �Ⱥ�(��ֵ)
	EQUAL_EQUAL,    // ==  ��ȱȽ�
	GREATER,        // >   ���ں�
	GREATER_EQUAL,  // >=  ���ڵ���
	LESS,           // <   С�ں�
	LESS_EQUAL,     // <=  С�ڵ���

	// ������
	IDENTIFIER,     //     ��ʶ��(������/��������)
	STRING,         //     �ַ���������
	NUMBER,         //     ����������

	// �ؼ���
	AND,            // and     �߼���
	CLASS,          // class   �ඨ��
	ELSE,           // else    if����else��֧
	FALSE,          // false   ������ֵ
	FUN,            // fun     ��������
	FOR,            // for     forѭ��
	IF,             // if      if�������
	NIL,            // nil     ��ֵ
	OR,             // or      �߼���
	PRINT,          // print   ��ӡ���
	RETURN,         // return  ��������
	SUPER,          // super   ��������
	THIS,           // this    ��ǰ��������
	TRUE,           // true    ������ֵ
	VAR,            // var     ��������
	WHILE,          // while   whileѭ��

	END             //         Դ����������
};

map<TokenType, string> trans;

//rain���� �ʷ����������ת��ΪToken��
//���ߣ�xiao
//github:3174378573xiao
//qq:3174378573
class Token
{
public:
	TokenType type;
	string lexeme;//���� ��˫����
#define object variant<double , string ,int>
	object literal;//ԭ����˼
	int line;
	Token(TokenType type, string lexeme, object literal, int line)
		: type(type), lexeme(lexeme), literal(literal), line(line) {
			{
				// ���ַ����
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
				//һ�������ַ��ı��
				trans[BANG] = "BANG";
				trans[BANG_EQUAL] = "BANG_EQUAL";
				trans[EQUAL] = "EQUAL";
				trans[EQUAL_EQUAL] = "EQUAL_EQUAL";
				trans[GREATER] = "GREATER";
				trans[GREATER_EQUAL] = "GREATER_EQUAL";
				trans[LESS] = "LESS";
				trans[LESS_EQUAL] = "LESS_EQUAL";
				//������
				trans[IDENTIFIER] = "IDENTIFIER";
				trans[STRING] = "STRING";
				trans[NUMBER] = "NUMBER";
				//�ؼ���
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
			// �����������ȥ��С��������ʾ
			if (literalString.find('.') != string::npos) {
				// ȥ�������0
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

		// ʹ�ù̶���ȸ�ʽ�����
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