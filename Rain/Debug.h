
//rain语言 编译器中方便调试的函数
//作者：xiao
//github:3174378573xiao
//qq:3174378573#pragma once
#pragma once
#include <iostream>
#include <ostream>
using namespace std;

//debug_log
//当未定义DEBUG宏时，debug_log语句不会输出，用于调试
//用法与cout完全相同
#ifdef DEBUG
// 定义 DEBUG 时，dout 重定向到 std::cerr（或 std::debug_log）
#define debug_log std::cerr
#else
// 未定义 DEBUG 时，使用空流对象
class NullStream {
public:
	// 处理所有类型的输出（无操作）
	template<typename T>
	NullStream& operator<<(const T&) {
		return *this;
	}

	// 处理流操作符（如 std::endl、std::hex 等）
	NullStream& operator<<(std::ostream& (*)(std::ostream&)) {
		return *this;
	}
};

// 全局空流实例
inline NullStream null_stream;
#define debug_log null_stream
#endif



//打印传入参数的信息
//argc 传入参数的数量，argv传入参数的数组
void argPrint(int argc, char* argv[]) {
	debug_log << "argc（传入参数数量）:" << argc << endl;
	//debug_log <<  << argv[0] << endl;
	debug_log << "argv(传入参数)：" << endl;
	for (int i = 0; i < argc; i++)
	{
		debug_log << i << "|";
		if (i == 0) debug_log << "编译器的路径:";
		debug_log << argv[i] << endl;
	}
}
