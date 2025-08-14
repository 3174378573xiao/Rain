//rain语言
//作者：xiao
//github:3174378573xiao
//qq:3174378573

#define DEBUG

//rain语言编译器的入口处
#include<iostream>
#include<fstream>
#include<string>
#include "Debug.h"//关于调试输出的内容 需要 c++17
#include "Scanner.h"
using namespace std;

//argc 传入参数的数量，argv传入参数的数组
int main(int argc, char* argv[]) {
	//打印传入参数的信息
	debug_log << string(20, '-') << endl;
	argPrint(argc, argv);
	debug_log << string(20, '-') << endl;
	//判断模式
	switch (argc)
	{
	case 1:
		//交互模式
		debug_log << "当前模式：交互模式" << endl;
		//只有默认的1个地址参数，即没有传入参数

		//TODO

		break;

	case 2:
		//除了默认的1个地址参数，还有一个参数

		//通过参数首字符判断模式
		if (argv[1][0] == '-') {
			//帮助模式
			debug_log << "当前模式：帮助模式" << strcmp(argv[1], "-help") << endl;
			//string temp = argv[1];
			if (!strcmp(argv[1], "-help")) {
				//帮助
				cout << "输入 rain 进入交互模式" << endl;
				cout << "输入 rain [需编译的文件] 进入编译模式" << endl;
				cout << "输入 rain -about 了解更多(无用)信息" << endl;
			}
			else if (!strcmp(argv[1], "-about")) {
				//关于
				cout << "这是rain语言的编译器" << endl;
				cout << "作者：xiao" << endl;
				cout << "github:3174378573xiao" << endl;
				cout << "qq:3174378573" << endl;
				cout << "如你所见，这是作者无聊时写的东西。作者的英语不好，所以。。。\n总之，从零实现自己的编程语言还是很有意思和成就感的。\n";
				cout << "什么？你问为什叫rain？ Because rain sounds great.Right? ...听雨\n";
			}

		}
		else {
			//编译模式
			debug_log << "当前模式：编译模式" << endl;

			//读取文件
			fstream file;
			file.open(argv[1], ios::in);
			if (!file.is_open())cout << "[错误]无法打开文件:" << argv[1] << endl;
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
		//传入参数数量不合法
		//输出警告/错误信息,提供帮助信息
		cout << "[错误]:传入参数不合法,输入 rain -help 获得帮助" << endl;
		break;
	}
	return 0;
}
