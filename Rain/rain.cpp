//rain����
//���ߣ�xiao
//github:3174378573xiao
//qq:3174378573

#define DEBUG

//rain���Ա���������ڴ�
#include<iostream>
#include<fstream>
#include<string>
#include "Debug.h"//���ڵ������������ ��Ҫ c++17
#include "Scanner.h"
using namespace std;

//argc ���������������argv�������������
int main(int argc, char* argv[]) {
	//��ӡ�����������Ϣ
	debug_log << string(20, '-') << endl;
	argPrint(argc, argv);
	debug_log << string(20, '-') << endl;
	//�ж�ģʽ
	switch (argc)
	{
	case 1:
		//����ģʽ
		debug_log << "��ǰģʽ������ģʽ" << endl;
		//ֻ��Ĭ�ϵ�1����ַ��������û�д������

		//TODO

		break;

	case 2:
		//����Ĭ�ϵ�1����ַ����������һ������

		//ͨ���������ַ��ж�ģʽ
		if (argv[1][0] == '-') {
			//����ģʽ
			debug_log << "��ǰģʽ������ģʽ" << strcmp(argv[1], "-help") << endl;
			//string temp = argv[1];
			if (!strcmp(argv[1], "-help")) {
				//����
				cout << "���� rain ���뽻��ģʽ" << endl;
				cout << "���� rain [�������ļ�] �������ģʽ" << endl;
				cout << "���� rain -about �˽����(����)��Ϣ" << endl;
			}
			else if (!strcmp(argv[1], "-about")) {
				//����
				cout << "����rain���Եı�����" << endl;
				cout << "���ߣ�xiao" << endl;
				cout << "github:3174378573xiao" << endl;
				cout << "qq:3174378573" << endl;
				cout << "����������������������ʱд�Ķ��������ߵ�Ӣ�ﲻ�ã����ԡ�����\n��֮������ʵ���Լ��ı�����Ի��Ǻ�����˼�ͳɾ͸еġ�\n";
				cout << "ʲô������Ϊʲ��rain�� Because rain sounds great.Right? ...����\n";
			}

		}
		else {
			//����ģʽ
			debug_log << "��ǰģʽ������ģʽ" << endl;

			//��ȡ�ļ�
			fstream file;
			file.open(argv[1], ios::in);
			if (!file.is_open())cout << "[����]�޷����ļ�:" << argv[1] << endl;
			else {
				string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
				
				Scanner scanner(source);

				debug_log << string(20, '-') << endl;
				debug_log << source << endl;
				debug_log << string(20, '-') << endl;
				file.close();
			}

			//TODO
		}
		break;

	default:
		//��������������Ϸ�
		//�������/������Ϣ,�ṩ������Ϣ
		cout << "[����]:����������Ϸ�,���� rain -help ��ð���" << endl;
		break;
	}
	return 0;
}
