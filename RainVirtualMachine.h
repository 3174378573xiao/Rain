//rain语言 编译中执行编译后字节码的虚拟机
//作者：xiao
//github:3174378573xiao
//qq:3174378573

#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <stack>
#include <exception>
#include <unordered_map>
#include <variant>  // 用于存储多种类型的值
#include <stdexcept> // 异常处理
#include <cctype>   // 字符处理
#include <memory>   // 智能指针
#include <functional> // 函数对象

// 定义虚拟机支持的数据类型：布尔值、双精度浮点数、字符串
using Value = std::variant<bool, double, std::string>;

// 指令集枚举 - 定义虚拟机支持的所有操作
enum class OpCode {
	// 操作						对应参数
	PUSH,       // 将值压入栈顶				要压入的值
	POP,        // 弹出栈顶值
	LOAD,       // 加载变量到栈顶				要加载的变量名
	STORE,      // 将栈顶值存储到变量			要储存的变量名
	CALL,       // 调用函数					要调用的函数名称
	RET,        // 函数返回					
	ADD,        // 加法
	SUB,        // 减法
	MUL,        // 乘法
	DIV,        // 除法
	CONCAT,     // 字符串连接
	EQ,         // 相等比较
	LT,         // 小于比较
	JMP,        // 无条件跳转
	JMP_IF,     // 条件为真时跳转
	JMP_IFNOT,  // 条件为假时跳转
	PRINT,      // 打印栈顶值
	HALT        // 停止执行
};

// 指令结构：包含操作码和操作数
struct Instruction {
	OpCode op;  // 操作类型
	std::variant<int, double, std::string, bool> operand; // 操作数（支持多种类型）

	// 构造函数 - 无操作数
	Instruction(OpCode op) : op(op), operand(0) {}

	// 构造函数 - 带操作数（使用模板支持不同类型）
	template <typename T>
	Instruction(OpCode op, T operand) : op(op), operand(operand) {}

	std::string to_string() {
		// 获取操作码名称
		std::string opName;
		switch (op) {
		case OpCode::PUSH: opName = "PUSH"; break;
		case OpCode::POP: opName = "POP"; break;
		case OpCode::LOAD: opName = "LOAD"; break;
		case OpCode::STORE: opName = "STORE"; break;
		case OpCode::CALL: opName = "CALL"; break;
		case OpCode::RET: opName = "RET"; break;
		case OpCode::ADD: opName = "ADD"; break;
		case OpCode::SUB: opName = "SUB"; break;
		case OpCode::MUL: opName = "MUL"; break;
		case OpCode::DIV: opName = "DIV"; break;
		case OpCode::CONCAT: opName = "CONCAT"; break;
		case OpCode::EQ: opName = "EQ"; break;
		case OpCode::LT: opName = "LT"; break;
		case OpCode::JMP: opName = "JMP"; break;
		case OpCode::JMP_IF: opName = "JMP_IF"; break;
		case OpCode::JMP_IFNOT: opName = "JMP_IFNOT"; break;
		case OpCode::PRINT: opName = "PRINT"; break;
		case OpCode::HALT: opName = "HALT"; break;
		default: opName = "UNKNOWN"; break;
		}

		// 处理操作数
		std::string operandStr;
		if (std::holds_alternative<int>(operand)) {
			operandStr = std::to_string(std::get<int>(operand));
		}
		else if (std::holds_alternative<double>(operand)) {
			operandStr = std::to_string(std::get<double>(operand));
		}
		else if (std::holds_alternative<std::string>(operand)) {
			operandStr = std::get<std::string>(operand);
		}
		else if (std::holds_alternative<bool>(operand)) {
			operandStr = std::get<bool>(operand) ? "true" : "false";
		}
		else {
			operandStr = "?";
		}

		return opName + " " + operandStr;
	}
};

// 函数定义结构：包含指令序列和参数个数
struct Function {

	// 函数指令序列
	std::vector<Instruction> instructions;

	std::vector<std::string> paramNames;

	// 函数返回值 个数
	int returnNum = 0;

	//函数名
	std::string funcName;
};

// 调用栈帧结构 - 存储函数调用信息
struct CallFrame {

	// 应返回到的地址（下一条指令位置）
	size_t return_address;

	// 应返回到的函数
	Function* return_function;

	// 局部变量表
	std::unordered_map<std::string, Value> locals;
};


//RainVM包含：
//1、Current Function(当前执行的函数):			储存当前执行的函数的rvmCode指令的vector 
//2、CFP(Current Function Counter程序计数器)：	始终保存下一条要执行指令的地址
// 
//3、Operal Stack（操作栈）：						用于计算、缓存的临时空间，同时用于函数调用时的传参
// 
//4、Call Stack(函数栈)：					用于保存函数调用时产生的栈帧(包括要返回的地址，局部变量表)
//
//5、GVT(Global Variable Table全局变量表)：		储存全局变量
//6、GFT(Global Function Table 全局函数表)：		储存注册的函数
class RainVirtualMachine
{
public:
	// 添加构造函数
	RainVirtualMachine()
		: CurrentFunction(nullptr)
		, CurrentFunctionCounter(0)
	{}
	//~RainVirtualMachine();

	//注册全局变量 成功返回true
	bool registerGlobalVariable(std::string valueName, Value value);

	//注册函数 成功返回true
	bool registerGlobalFunction(Function function);

	//打印全局变量表
	void displayGlobalVariableTable();

	//打印全局函数表
	void displayGlobalFunctionTable();

	//运行虚拟机
	void run();

	void executeInstruction(Instruction instuction);


private:
	//1、Current Function(当前执行的函数):			储存当前执行的函数的rvmCode指令的vector 
	Function* CurrentFunction;

	//2、CFP(Current Function Counter程序计数器)：	始终保存下一条要执行指令的地址
	size_t CurrentFunctionCounter;

	//3、Operal Stack（操作栈）：						用于计算、缓存的临时空间，同时用于函数调用时的传参
	std::stack<Value> OperalStack;

	//4、Call Stack(函数栈)：					用于保存函数调用时产生的栈帧(包括要返回的地址，局部变量表)
	std::stack<CallFrame> CallStack;

	//5、GVT(Global Variable Table全局变量表)：		储存全局变量
	std::unordered_map<std::string, Value> GlobalVariableTable;

	//6、GFT(Global Function Table 全局函数表)：		储存注册的函数
	std::unordered_map<std::string, Function> GlobalFunctionTable;


	struct RunningTimeException : public std::exception
	{
		std::string content;
		std::string message;  // 存储完整的错误信息
		RainVirtualMachine* vm;
		RunningTimeException(std::string content, RainVirtualMachine* vm) :content(content), vm(vm) {
			message = "[错误]: 在函数\"" + vm->CurrentFunction->funcName + "\"中\n";
			size_t index = (vm->CurrentFunctionCounter < vm->CurrentFunction->instructions.size())
				? vm->CurrentFunctionCounter
				: vm->CurrentFunction->instructions.size() - 1;

			message += "\t指令 #" + std::to_string(index) + ": " +
				vm->CurrentFunction->instructions[index].to_string() + "\n";
			message += "\t" + content + "\n";

			// 添加调用栈信息
			if (!vm->CallStack.empty()) {
				message += "调用栈:\n";
				auto tempStack = vm->CallStack;
				int level = 1;
				while (!tempStack.empty()) {
					auto& frame = tempStack.top();
					message += "\t#" + std::to_string(level++) + " " +
						frame.return_function->funcName + "\n";
					tempStack.pop();
				}
			}
		}
		const char* what() const throw ()
		{
			return message.c_str();
		}
	};
};