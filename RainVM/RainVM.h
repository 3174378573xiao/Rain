#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <variant>
#include <iomanip>
#include <stack>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;
enum Op {

	LOAD,//将变量加载到栈顶
	SAVE,//将栈顶值保存到变量表
	PUSH, //将值压入栈顶
	POP, //将值压入栈顶
	PRINT,//打印并弹出栈顶值
	INPUT,
	DUP,//将栈顶值复制一份

	ADD,  // 加法: 弹出栈顶两个元素，相加后压回结果
	SUB,  // 减法: a = 次栈顶, b = 栈顶, 压入 a-b
	MUL,  // 乘法
	DIV,  // 除法
	MOD,  // 取模
	NEG,  // 取负: 单目运算，弹出栈顶元素取负后压回

	EQ,   // 等于: 弹出两个元素，比较后压入布尔结果
	NEQ,  // 不等于
	GT,   // 大于
	LT,   // 小于
	GTE,  // 大于等于
	LTE,  // 小于等于
	AND,  // 逻辑与
	OR,   // 逻辑或
	NOT,  // 逻辑非

	JMP,      // 无条件跳转: 参数为跳转地址
	JMP_IF,   // 条件为真跳转
	JMP_NOT,  // 条件为假跳转
	CALL,     // 函数调用: 参数为函数名
	RET,      // 函数返回
	HALT,     // 停止执行
};

using Value = variant<int, double, string, bool>;
struct Instruction
{
	Op op;
	Value arg;
	// 在 struct Instruction 内部添加 to_string 方法实现
	string toString() {
		// 定义操作码名称映射表
		static const unordered_map<Op, string> opNames = {
			{LOAD, "LOAD"},
			{SAVE, "SAVE"},
			{PUSH, "PUSH"},
			{POP, "POP"},
			{PRINT, "PRINT"},
			{INPUT, "INPUT"},
			{DUP, "DUP"},
			{ADD, "ADD"},
			{SUB, "SUB"},
			{MUL, "MUL"},
			{DIV, "DIV"},
			{MOD, "MOD"},
			{NEG, "NEG"},
			{EQ, "EQ"},
			{NEQ, "NEQ"},
			{GT, "GT"},
			{LT, "LT"},
			{GTE, "GTE"},
			{LTE, "LTE"},
			{AND, "AND"},
			{OR, "OR"},
			{NOT, "NOT"},
			{JMP, "JMP"},
			{JMP_IF, "JMP_IF"},
			{JMP_NOT, "JMP_NOT"},
			{CALL, "CALL"},
			{RET, "RET"},
			{HALT, "HALT"}
		};

		string result = opNames.at(op);

		// 添加参数表示
		if (op == PUSH || op == JMP || op == JMP_IF || op == JMP_NOT ||
			op == CALL || op == LOAD || op == SAVE) {

			// 访问者模式处理不同类型
			auto visiter = [](auto&& arg) -> string {
				using T = decay_t<decltype(arg)>;
				if constexpr (is_same_v<T, int>) {
					return to_string(arg);
				}
				else if constexpr (is_same_v<T, double>) {
					return to_string(arg);
				}
				else if constexpr (is_same_v<T, string>) {
					return "\"" + arg + "\"";
				}
				else if constexpr (is_same_v<T, bool>) {
					return arg ? "true" : "false";
				}
				};

			result += " " + visit(visiter, arg);
		}

		return result;
	}
};

struct CallFrame
{
	//返回的地址
	unsigned int return_pc = 0;

	string return_func;

	//局部变量表
	unordered_map<string, Value> LVT;
};

// 函数定义结构：包含指令序列和参数个数
struct Function {

	// 函数指令序列
	vector<Instruction> instructions;

	vector<string> paramNames;

};

class RainVM
{
public:
	RainVM() : pc(0), c_func(), c_func_name(""), OpStack(), CallStack(), GVT(), GFT() ,is_end(false){}
	//注册全局变量 成功返回true
	bool registerGlobalVariable(string valueName, Value value) {
		return registerVariable(GVT, valueName, value);
	}

	//注册函数 成功返回true
	bool registerGlobalFunction(string funcName, Function function) {
		if (!isVariableName(funcName)) {
			std::cout << "[错误]:全局函数\"" + funcName + "\"的名称不合法\n";
			return false;
		}
		if (GFT.find(funcName) != GFT.end()) {
			//如果已注册 报错
			std::cout << "[错误]:全局函数\"" + funcName + "\"已被注册\n";
			return false;
		}
		else {
			//如果未注册 注册
			GFT.emplace(funcName, function);
			return true;
		}
	}

	//打印全局变量表
	void displayGlobalVariableTable() {
		cout << "---GlobalVariableTable(全局变量表)---" << std::endl;
		cout << std::setw(10) << "变量名" << std::setw(20) << "值" << std::endl;

		for (auto i : GVT) {

			//使用固定宽度格式化输出
			cout << std::setw(10) << i.first.c_str() << std::setw(20);
			visit(print_visiter{}, i.second);
			cout << std::endl;
		}
		cout << "------------------------------------" << std::endl;
	}

	//打印全局函数表
	void displayGlobalFunctionTable() {
		std::cout << "---GlobalFunctionTable(全局函数表)--" << std::endl;
		std::cout << std::setw(10) << "函数名" << std::setw(15) << "指令个数" << std::endl;
		for (auto i : GFT) {
			std::cout << std::setw(10) << i.first.c_str() << std::setw(15) << i.second.instructions.size() << std::endl;
		}
		std::cout << "------------------------------------" << std::endl;
	}

	//运行虚拟机
	void run() {

		if (GFT.find("main") == GFT.end()) {
			std::cout << "[错误]: 找不到入口函数 \"main\"\n";
			return;
		}

		// 初始化执行环境
		pc = 0;
		while (!CallStack.empty()) CallStack.pop();
		c_func = GFT["main"];
		c_func_name = "main";

		vector<Instruction> code = c_func.instructions;
		while (pc < code.size()&&(!is_end)) {
			//cout << setw(4) << pc << setw(4) << code[pc].op<< "|";
			execute(code);
			//cout << endl;
		}
	}

	//执行单条指令
	void execute(vector<Instruction>& code) {
		Instruction ins = code[pc++];
		switch (ins.op)
		{
		case LOAD:	load(ins);	break;
		case SAVE:  save(ins);  break;
		case PUSH:	push(ins);	break;
		case POP:OpStack.pop();	break;
		case PRINT:print(ins);	break;
		case INPUT:input(ins);	break;
		case DUP:	OpStack.push(OpStack.top()); break;

		case ADD:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a + b; }); break;
		case SUB:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a - b; }); break;
		case MUL:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a * b; }); break;
		case DIV:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a / b; }); break;
		case MOD:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return fmod(a, b); }); break;
		case NEG:	unaryCalculate(ins, Number_visiter{}, [](double i) {return -i; }); break;

		case EQ:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a == b; }); break;
		case NEQ:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a != b; }); break;
		case GT:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a > b; }); break;
		case LT:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a < b; }); break;
		case GTE:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a >= b; }); break;
		case LTE:	binaryCalculate(ins, Number_visiter{}, [](double a, double b) {return a <= b; }); break;

		case AND:	binaryCalculate(ins, Boolean_visiter{}, [](bool a, bool b) {return a && b; }); break;
		case OR:	binaryCalculate(ins, Boolean_visiter{}, [](bool a, bool b) {return a || b; }); break;
		case NOT:	unaryCalculate(ins, Boolean_visiter{}, [](bool i) {return !i; }); break;

		case JMP:   jump(ins, true); break;
		case JMP_IF:jump(ins, get<bool>(OpStack.top())); break;
		case JMP_NOT:jump(ins, !get<bool>(OpStack.top())); break;

		case CALL: call(ins,code); break;
		case RET: ret(ins,code); break;
		case HALT: is_end = true; break;
		default:
			break;
		}
	}

private:

	bool is_end = false;
	//下一条指令的地址
	unsigned int pc = 0;

	//当前执行的函数
	Function c_func;

	string c_func_name;

	//操作栈
	stack<Value> OpStack;

	//调用栈
	stack<CallFrame> CallStack;

	//全局变量表
	unordered_map<string, Value> GVT;

	//全局函数表
	unordered_map<string, Function> GFT;


	void push(Instruction ins) {
		OpStack.push(ins.arg);
	}

	struct print_visiter
	{
		void operator()(int i) { cout << i; }
		void operator()(double i) { cout << i; }
		void operator()(string i) { cout << i; }
		void operator()(bool i) { cout << (i ? "true" : "false"); }
	};

	void print(Instruction ins) {

		visit(print_visiter{}, OpStack.top());
		OpStack.pop();
	}

	void input(Instruction ins) {
		string sin;
		getline(cin, sin);
		// 尝试解析为整数
		try {
			size_t pos = 0;
			int intValue = stoi(sin, &pos);
			if (pos == sin.size()) {
				OpStack.push(intValue);
				return;
			}
		}
		catch (...) {}

		// 尝试解析为浮点数
		try {
			size_t pos = 0;
			double doubleValue = stod(sin, &pos);
			if (pos == sin.size())
			{
				OpStack.push(doubleValue);
				return;
			}
		}
		catch (...) {}


		OpStack.push(sin);
		return;
	}

	struct Number_visiter
	{
		double operator()(int i) { return i; }
		double operator()(double i) { return i; }
		double operator()(string s) {
			double r = 0;
			try {
				r = stod(s);
			}
			catch (...) {
				throw "[错误]:字符串无法转换为数字";
			}
			return r;
		}
		double operator()(bool b) {
			throw "[错误]:布尔值无法转换为数字";
			return b ? 1.0 : 0.0;  // 布尔值转换为数字
		}
	};

	struct Boolean_visiter
	{
		bool operator()(int i) { return !(i == 0); }
		bool operator()(double i) { return !(i == 0.0); }
		bool operator()(string s) {
			double r = 0;
			try {
				transform(s.begin(), s.end(), s.begin(), ::tolower);
				if (!strcmp(s.c_str(), "true"))return true;
				if (!strcmp(s.c_str(), "false"))return false;
			}
			catch (...) {
				throw "[错误]:字符串无法转换为数字";
			}
			return true;
		}
		bool operator()(bool b) {
			return b;
		}
	};

	void unaryCalculate(Instruction ins, auto visiter, auto op) {

		double i = visit(visiter, OpStack.top());
		OpStack.pop();
		Value r = op(i);
		OpStack.push(r);
	}

	void binaryCalculate(Instruction ins, auto visiter, auto op) {

		double b = visit(visiter, OpStack.top());
		OpStack.pop();
		double a = visit(visiter, OpStack.top());
		OpStack.pop();
		Value r = op(a, b);
		OpStack.push(r);
	}

	void save(Instruction ins) {
		string valueName = get<string>(ins.arg);
		if (CallStack.empty()) {
			//全局环境
			if (GVT.find(valueName) == GVT.end()) {
				registerVariable(GVT, valueName, OpStack.top());
			}
			else {
				GVT[valueName] = OpStack.top();
			}
		}
		else {
			auto& LVT = CallStack.top().LVT;
			//局部环境
			if (LVT.find(valueName) == LVT.end()) {
				registerVariable(LVT, valueName, OpStack.top());
			}
			else {
				LVT[valueName] = OpStack.top();
			}
		}
		OpStack.pop();
	}

	void load(Instruction ins) {
		string valueName = get<string>(ins.arg);
		if (!CallStack.empty()) {
			auto& LVT = CallStack.top().LVT;
			if (LVT.find(valueName) != LVT.end()) {
				OpStack.push(LVT[valueName]);
				return;
			}
		}
		else if (GVT.find(valueName) != GVT.end()) {
			OpStack.push(GVT[valueName]);
			return;
		}
		else {
			OpStack.push(0);
			cout << "[错误]:变量" << valueName << "不存在\n";
		}
	}

	void jump(Instruction ins, bool when) {
		unsigned int go_pc = (unsigned int) visit(Number_visiter{}, ins.arg);
		if (when) {
			pc = go_pc;
			OpStack.pop();
		}
	}

	void call(Instruction ins,vector<Instruction>& code){
		string call_func_name = get<string>(ins.arg);
		Function call_func =  GFT[call_func_name];
		CallFrame callFrame;
		callFrame.return_pc = pc;
		callFrame.return_func = c_func_name;

		for (auto i = call_func.paramNames.rbegin();i!= call_func.paramNames.rend();i++) {
			registerVariable(callFrame.LVT, *i, OpStack.top());
			OpStack.pop();
		}
		CallStack.push(callFrame);

		pc = 0;
		c_func = call_func;
		c_func_name = call_func_name;
		code = call_func.instructions;
	}

	void ret(Instruction ins, vector<Instruction>& code) {
		CallFrame retFrame = CallStack.top();
		
		c_func = GFT[retFrame.return_func];
		c_func_name = retFrame.return_func;
		pc = retFrame.return_pc;
		code = c_func.instructions;
		
		CallStack.pop();
	}


	//判断名字是否合法
	bool isVariableName(const std::string& name) {
		// 情况1：空字符串直接不合法
		if (name.empty()) {
			return false;
		}

		// 情况2：检查第一个字符
		// 合法首字符：字母或下划线
		if (!isalpha(name[0]) && name[0] != '_') {
			return false;
		}

		// 情况3：检查后续字符
		// 合法字符：字母、数字或下划线
		for (size_t i = 1; i < name.size(); ++i) {
			if (!isalnum(name[i]) && name[i] != '_') {
				return false;
			}
		}
		// 所有检查都通过，是合法变量名
		return true;
	}

	bool registerVariable(unordered_map<string, Value>& table, string valueName, Value value) {
		if (!isVariableName(valueName)) {
			std::cout << "[错误]:变量\"" + valueName + "\"的名称不合法\n";
			return false;
		}
		if (table.find(valueName) != table.end()) {
			//如果已注册 报错
			std::cout << "[错误]:变量\"" + valueName + "\"已被注册\n";
			return false;
		}
		else {
			//如果未注册 注册
			table.emplace(valueName, value);
			return true;
		}
	}
};