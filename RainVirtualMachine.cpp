#include "RainVirtualMachine.h"

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

bool RainVirtualMachine::registerGlobalVariable(std::string valueName, Value value)
{
	if (!isVariableName(valueName)) {
		std::cout << "[错误]:全局变量\"" + valueName + "\"的名称不合法\n";
		return false;
	}
	if (this->GlobalVariableTable.find(valueName) != this->GlobalVariableTable.end()) {
		//如果已注册 报错
		std::cout << "[错误]:全局变量\"" + valueName + "\"已被注册\n";
		return false;
	}
	else {
		//如果未注册 注册
		this->GlobalVariableTable.emplace(valueName, value);
		//std::cout << "ok\n" ;
		return true;
	}
}

bool RainVirtualMachine::registerGlobalFunction(Function function)
{
	if (!isVariableName(function.funcName)) {
		std::cout << "[错误]:全局函数\"" + function.funcName + "\"的名称不合法\n";
		return false;
	}
	if (this->GlobalFunctionTable.find(function.funcName) != this->GlobalFunctionTable.end()) {
		//如果已注册 报错
		std::cout << "[错误]:全局函数\"" + function.funcName + "\"已被注册\n";
		return false;
	}
	else {
		//如果未注册 注册
		this->GlobalFunctionTable.emplace(function.funcName, function);
		return true;
	}
}

void RainVirtualMachine::displayGlobalVariableTable()
{
	std::cout << "---GlobalVariableTable(全局变量表)---" << std::endl;
	std::cout << std::setw(10) << "变量名" << std::setw(20) << "值" << std::endl;
	for (auto i : this->GlobalVariableTable) {
		std::string temp;
		// 识别变量类型
		if (std::holds_alternative<double>(i.second)) {
			double t1 = std::get<double>(i.second);

			//整数
			if (fmod(t1, 1.0) == 0.0) {
				temp = std::to_string(int(t1));
			}
			else {
				temp = std::to_string(double(t1));
			}
		}
		else if (std::holds_alternative<bool>(i.second)) {
			bool t2 = std::get<bool>(i.second);
			temp = t2 ? "true" : "false";
		}
		else if (std::holds_alternative<std::string>(i.second)) {
			temp = std::get<std::string>(i.second);
		}
		//使用固定宽度格式化输出
		std::cout << std::setw(10) << i.first.c_str() << std::setw(20) << temp.c_str() << std::endl;
	}
	std::cout << "------------------------------------" << std::endl;
}

void RainVirtualMachine::displayGlobalFunctionTable()
{
	std::cout << "---GlobalFunctionTable(全局函数表)--" << std::endl;
	std::cout << std::setw(10) << "函数名" << std::setw(15) << "参数个数" << std::setw(15) << "返回值个数" << std::endl;
	for (auto i : this->GlobalFunctionTable) {
		std::cout << std::setw(10) << i.first.c_str() << std::setw(15) << i.second.paramNames.size() << std::setw(15) << i.second.returnNum << std::endl;
	}
	std::cout << "------------------------------------" << std::endl;
}

void RainVirtualMachine::run()
{
	if (this->GlobalFunctionTable.find("main") == this->GlobalFunctionTable.end()) {
		std::cout << "[错误]: 找不到入口函数 \"main\"\n";
		return;
	}

	// 初始化执行环境
	this->CurrentFunctionCounter = 0;
	while (!this->CallStack.empty()) this->CallStack.pop();
	while (!this->OperalStack.empty()) this->OperalStack.pop();
	this->CurrentFunction = &(this->GlobalFunctionTable["main"]);

	try {
		while (this->CurrentFunctionCounter < this->CurrentFunction->instructions.size()) {
			// 执行当前指令
			executeInstruction(this->CurrentFunction->instructions[this->CurrentFunctionCounter]);

			// 默认情况下，程序计数器自增
			// 跳转指令会修改程序计数器，因此不需要自增
			if (this->CurrentFunction->instructions[this->CurrentFunctionCounter].op != OpCode::JMP &&
				this->CurrentFunction->instructions[this->CurrentFunctionCounter].op != OpCode::JMP_IF &&
				this->CurrentFunction->instructions[this->CurrentFunctionCounter].op != OpCode::JMP_IFNOT &&
				this->CurrentFunction->instructions[this->CurrentFunctionCounter].op != OpCode::CALL &&
				this->CurrentFunction->instructions[this->CurrentFunctionCounter].op != OpCode::RET) {
				this->CurrentFunctionCounter++;
			}
		}
	}
	catch (const RunningTimeException& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "[运行时错误]: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "[未知运行时错误]" << std::endl;
	}
}

auto to_string = [](const Value& v) {
	if (std::holds_alternative<std::string>(v)) {
		return std::get<std::string>(v);
	}
	if (std::holds_alternative<double>(v))
	{
		return std::to_string(std::get<double>(v));
	}
	if (std::holds_alternative<bool>(v))
	{
		std::string t = std::get<bool>(v) ? "true" : "false";
		return t;
	}
	else {
		return std::string();
	}
};
void RainVirtualMachine::executeInstruction(Instruction instruction)
{
	switch (instruction.op) {
	case OpCode::PUSH: {
		// 处理不同类型的压栈操作
		if (std::holds_alternative<bool>(instruction.operand)) {
			OperalStack.push(std::get<bool>(instruction.operand));
		}
		else if (std::holds_alternative<int>(instruction.operand)) {
			OperalStack.push(static_cast<double>(std::get<int>(instruction.operand)));
		}
		else if (std::holds_alternative<double>(instruction.operand)) {
			OperalStack.push(std::get<double>(instruction.operand));
		}
		else if (std::holds_alternative<std::string>(instruction.operand)) {
			OperalStack.push(std::get<std::string>(instruction.operand));
		}
		break;
	}

	case OpCode::POP: {
		if (OperalStack.empty()) {
			throw RunningTimeException("栈下溢错误", this);
		}
		OperalStack.pop();
		break;
	}

	case OpCode::LOAD: {
		auto varName = std::get<std::string>(instruction.operand);

		// 1. 检查当前函数的局部变量
		if (!CallStack.empty() && CallStack.top().locals.count(varName)) {
			OperalStack.push(CallStack.top().locals[varName]);
		}
		// 2. 检查全局变量
		else if (GlobalVariableTable.count(varName)) {
			OperalStack.push(GlobalVariableTable[varName]);
		}
		else {
			throw RunningTimeException("未定义的变量: " + varName, this);
		}
		break;
	}

	case OpCode::STORE: {
		if (OperalStack.empty()) {
			throw RunningTimeException("栈下溢错误", this);
		}

		auto varName = std::get<std::string>(instruction.operand);
		auto value = OperalStack.top();
		OperalStack.pop();

		// 优先存储在局部作用域
		if (!CallStack.empty()) {
			CallStack.top().locals[varName] = value;
		}
		// 没有局部作用域时存储在全局
		else {
			GlobalVariableTable[varName] = value;
		}
		break;
	}
	case OpCode::CALL: {
		auto funcName = std::get<std::string>(instruction.operand);

		// 检查函数是否存在
		if (GlobalFunctionTable.find(funcName) == GlobalFunctionTable.end()) {
			throw RunningTimeException("未定义的函数: " + funcName, this);
		}

		auto& func = GlobalFunctionTable[funcName];

		// 检查参数数量匹配
		if (OperalStack.size() < static_cast<size_t>(func.paramNames.size())) {
			throw RunningTimeException("函数调用参数不足: " + funcName, this);
		}

		// 创建新的调用帧
		CallFrame frame;
		frame.return_address = CurrentFunctionCounter + 1;
		frame.return_function = CurrentFunction;

		// 获取参数（按顺序弹出并存储到临时向量）
		std::vector<Value> args;
		args.reserve(func.paramNames.size());
		for (int i = 0; i < func.paramNames.size(); ++i) {
			if (OperalStack.empty()) {
				throw RunningTimeException("栈下溢错误", this);
			}
			args.push_back(OperalStack.top());
			OperalStack.pop();
		}

		// 修改后
		std::reverse(args.begin(), args.end());  // 反转参数顺序
		for (size_t i = 0; i < func.paramNames.size(); ++i) {
			frame.locals[func.paramNames[i]] = args[i];
		}

		CallStack.push(frame); // 压入调用栈

		// 切换到新函数
		CurrentFunction = &func;
		CurrentFunctionCounter = 0; // 从函数入口开始执行
		break;
	}

	case OpCode::RET: {

		if (CallStack.empty()) {
			// 主函数返回，结束执行
			CurrentFunctionCounter = CurrentFunction->instructions.size();
			return;
		}

		if (CallStack.empty()) {
			throw RunningTimeException("RET 指令在函数外使用", this);
		}

		// 检查返回值数量是否匹配
		Value returnValue;
		if (CurrentFunction->returnNum > 0) {
			if (OperalStack.empty()) {
				throw RunningTimeException("函数应有返回值但操作栈为空", this);
			}
			returnValue = OperalStack.top();
			OperalStack.pop();
		}

		// 恢复调用上下文
		auto frame = CallStack.top();
		CallStack.pop();

		CurrentFunction = frame.return_function;
		CurrentFunctionCounter = frame.return_address;

		// 压入返回值
		if (CurrentFunction->returnNum > 0) {
			OperalStack.push(returnValue);
		}
		break;
	}

	case OpCode::ADD: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}

		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();

		// 统一处理字符串和数字加法
		if (std::holds_alternative<std::string>(a) ||
			std::holds_alternative<std::string>(b))
		{
			OperalStack.push(to_string(a) + to_string(b));
		}
		// 处理数字加法
		else if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
			OperalStack.push(std::get<double>(a) + std::get<double>(b));
		}
		else {
			throw RunningTimeException("ADD 操作类型不匹配", this);
		}
		break;
	}
	case OpCode::SUB: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();
		// 类型检查
		if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
			OperalStack.push(std::get<double>(a) - std::get<double>(b));
		}
		else {
			throw RunningTimeException("SUB 操作类型不匹配", this);
		}
		break;
	}

	case OpCode::MUL: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();
		if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
			OperalStack.push(std::get<double>(a) * std::get<double>(b));
		}
		else {
			throw RunningTimeException("MUL 操作类型不匹配", this);
		}
		break;
	}

	case OpCode::DIV: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();
		if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
			double divisor = std::get<double>(b);
			if (divisor == 0) {
				throw RunningTimeException("除以零错误", this);
			}
			OperalStack.push(std::get<double>(a) / divisor);
		}
		else {
			throw RunningTimeException("DIV 操作类型不匹配", this);
		}
		break;
	}

	case OpCode::CONCAT: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();

		OperalStack.push(to_string(a) + to_string(b));
		break;
	}

	case OpCode::EQ: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();

		// 处理不同类型比较
		if (a.index() != b.index()) {
			// 不同类型比较为false
			OperalStack.push(false);
		}
		else {
			// 同类型比较
			if (std::holds_alternative<double>(a)) {
				OperalStack.push(std::get<double>(a) == std::get<double>(b));
			}
			else if (std::holds_alternative<bool>(a)) {
				OperalStack.push(std::get<bool>(a) == std::get<bool>(b));
			}
			else if (std::holds_alternative<std::string>(a)) {
				OperalStack.push(std::get<std::string>(a) == std::get<std::string>(b));
			}
			else {
				OperalStack.push(false);
			}
		}
		break;
	}

	case OpCode::LT: {
		if (OperalStack.size() < 2) {
			throw RunningTimeException("栈下溢错误", this);
		}
		auto b = OperalStack.top(); OperalStack.pop();
		auto a = OperalStack.top(); OperalStack.pop();

		if (std::holds_alternative<double>(a) && std::holds_alternative<double>(b)) {
			OperalStack.push(std::get<double>(a) < std::get<double>(b));
		}
		else if (std::holds_alternative<std::string>(a) && std::holds_alternative<std::string>(b)) {
			OperalStack.push(std::get<std::string>(a) < std::get<std::string>(b));
		}
		else {
			throw RunningTimeException("LT 操作类型不匹配", this);
		}
		break;
	}

	case OpCode::JMP_IFNOT: {
		if (OperalStack.empty()) {
			throw RunningTimeException("栈下溢错误", this);
		}
		if (!std::get<bool>(OperalStack.top())) {
			int target = std::get<int>(instruction.operand);
			if (target < 0 || target >= CurrentFunction->instructions.size()) {
				throw RunningTimeException("跳转地址越界: " + std::to_string(target), this);
			}
			CurrentFunctionCounter = std::get<int>(instruction.operand);
		}
		else {
			CurrentFunctionCounter++;
		}
		OperalStack.pop();
		break;
	}

	case OpCode::JMP: {
		int target = std::get<int>(instruction.operand);
		if (target < 0 || target >= CurrentFunction->instructions.size()) {
			throw RunningTimeException("跳转地址越界: " + std::to_string(target), this);
		}
		CurrentFunctionCounter = target;
		break;
	}

	case OpCode::JMP_IF: {
		if (OperalStack.empty()) {
			throw RunningTimeException("栈下溢错误", this);
		}

		if (std::get<bool>(OperalStack.top())) {
			int target = std::get<int>(instruction.operand);
			if (target < 0 || target >= CurrentFunction->instructions.size()) {
				throw RunningTimeException("跳转地址越界: " + std::to_string(target), this);
			}
			CurrentFunctionCounter = std::get<int>(instruction.operand);
		}
		else {
			CurrentFunctionCounter++; // 条件不成立则继续下一条
		}
		OperalStack.pop();
		break;
	}

	case OpCode::PRINT: {
		if (OperalStack.empty()) {
			throw RunningTimeException("栈下溢错误", this);
		}

		auto& top = OperalStack.top();
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, bool>) {
				std::cout << (arg ? "true" : "false") << std::endl;
			}
			else {
				std::cout << arg << std::endl;
			}
			}, top);

		OperalStack.pop(); // 添加弹出操作
		break;
	}

	case OpCode::HALT: {
		// 停止执行
		CurrentFunctionCounter = CurrentFunction->instructions.size();

		// 清理资源
		while (!CallStack.empty()) CallStack.pop();
		while (!OperalStack.empty()) OperalStack.pop();
		CurrentFunction = nullptr;
		break;
	}

	default: {
		throw RunningTimeException("未实现的指令", this);
	}
	}
}



