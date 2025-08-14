#include "RainVirtualMachine.h"
#include <cassert>

void testVariableRegistration() {
    RainVirtualMachine vm;
    // 测试合法变量名
    assert(vm.registerGlobalVariable("valid_var", 42.0));
    assert(vm.registerGlobalVariable("_valid2", true));
    assert(vm.registerGlobalVariable("var3", std::string("test")));

    // 测试非法变量名
    assert(!vm.registerGlobalVariable("", 0.0));  // 空名称
    assert(!vm.registerGlobalVariable("2invalid", 0.0));  // 数字开头
    assert(!vm.registerGlobalVariable("in valid", 0.0));  // 包含空格

    // 测试重复注册
    assert(!vm.registerGlobalVariable("valid_var", 100.0));

    std::cout << "变量注册测试通过\n";
}

void testFunctionRegistration() {
    RainVirtualMachine vm;
    Function mainFunc;
    mainFunc.funcName = "main";
    mainFunc.returnNum = 0;

    Function testFunc;
    testFunc.funcName = "test";
    testFunc.paramNames = { "a", "b" };
    testFunc.returnNum = 1;

    // 测试合法函数注册
    assert(vm.registerGlobalFunction(mainFunc));
    assert(vm.registerGlobalFunction(testFunc));

    // 测试非法函数名
    Function invalidFunc;
    invalidFunc.funcName = "invalid-name";
    assert(!vm.registerGlobalFunction(invalidFunc));

    // 测试重复注册
    assert(!vm.registerGlobalFunction(mainFunc));

    std::cout << "函数注册测试通过\n";
}

void testBasicOperations() {
    RainVirtualMachine vm;

    // 创建测试函数
    Function testFunc;
    testFunc.funcName = "test";
    testFunc.instructions = {
        {OpCode::PUSH, 10.0},
        {OpCode::PUSH, 5.0},
        {OpCode::ADD},      // 15
        {OpCode::PUSH, 3.0},
        {OpCode::MUL},      // 45
        {OpCode::PUSH, 9.0},
        {OpCode::DIV},      // 5
        {OpCode::PUSH, 2.0},
        {OpCode::SUB},      // 3
        {OpCode::STORE, "result"},
        {OpCode::LOAD, "result"},
        {OpCode::PRINT},
        {OpCode::HALT}
    };

    vm.registerGlobalFunction(testFunc);

    // 覆盖main函数
    Function main;
    main.funcName = "main";
    main.instructions = {
        {OpCode::CALL, "test"},
        {OpCode::HALT}
    };
    vm.registerGlobalFunction(main);

    vm.run();

    std::cout << "基础运算测试完成\n";
}

void testFunctionCall() {
    RainVirtualMachine vm;

    // 创建add函数
    Function addFunc;
    addFunc.funcName = "add";
    addFunc.paramNames = { "a", "b" };
    addFunc.returnNum = 1;
    addFunc.instructions = {
        {OpCode::LOAD, "a"},
        {OpCode::LOAD, "b"},
        {OpCode::ADD},
        {OpCode::RET}
    };

    // 创建main函数
    Function main;
    main.funcName = "main";
    main.instructions = {
        {OpCode::PUSH, 10.0},   // 参数b
        {OpCode::PUSH, 20.0},   // 参数a
        {OpCode::CALL, "add"},   // 调用add函数
        {OpCode::STORE, "sum"},  // 存储结果
        {OpCode::LOAD, "sum"},
        {OpCode::PRINT},         // 应打印30
        {OpCode::HALT}
    };

    vm.registerGlobalFunction(addFunc);
    vm.registerGlobalFunction(main);
    vm.run();

    std::cout << "函数调用测试完成\n";
}

void testConditionals() {
    RainVirtualMachine vm;

    // 创建判断函数
    Function check;
    check.funcName = "check";
    check.paramNames = { "value" };
    check.returnNum = 1;
    check.instructions = {
        {OpCode::LOAD, "value"},
        {OpCode::PUSH, 100.0},
        {OpCode::LT},            // value < 100
        {OpCode::JMP_IFNOT, 6},  // 不小于则跳转到false分支
        {OpCode::PUSH, std::string("small")},
        {OpCode::JMP, 8},        // 跳转到返回
        {OpCode::PUSH, std::string("big")},  // false分支
        {OpCode::RET}
    };

    // 创建main函数
    Function main;
    main.funcName = "main";
    main.instructions = {
        {OpCode::PUSH, 50.0},
        {OpCode::CALL, "check"},
        {OpCode::PRINT},  // 应打印"small"

        {OpCode::PUSH, 200.0},
        {OpCode::CALL, "check"},
        {OpCode::PRINT},  // 应打印"big"
        {OpCode::HALT}
    };

    vm.registerGlobalFunction(check);
    vm.registerGlobalFunction(main);
    vm.run();

    std::cout << "条件跳转测试完成\n";
}

void testErrorHandling() {
    RainVirtualMachine vm;

    // 1. 测试除以零
    Function divZero;
    divZero.funcName = "divZero";
    divZero.instructions = {
        {OpCode::PUSH, 10.0},
        {OpCode::PUSH, 0.0},
        {OpCode::DIV},  // 应触发除以零错误
        {OpCode::HALT}
    };

    // 2. 测试未定义变量
    Function undefVar;
    undefVar.funcName = "undefVar";
    undefVar.instructions = {
        {OpCode::LOAD, "undefined_var"},  // 应触发未定义变量错误
        {OpCode::HALT}
    };

    // 3. 测试栈下溢
    Function stackUnderflow;
    stackUnderflow.funcName = "stackUnderflow";
    stackUnderflow.instructions = {
        {OpCode::POP},  // 空栈弹出
        {OpCode::HALT}
    };

    // 创建main函数调用测试
    Function main;
    main.funcName = "main";
    main.instructions = {
        {OpCode::CALL, "divZero"},
        {OpCode::HALT}
    };

    std::cout << "\n测试除以零错误:\n";
    vm.registerGlobalFunction(divZero);
    vm.registerGlobalFunction(main);
    vm.run();

    RainVirtualMachine vm2;
    main.instructions[0].operand = std::string("undefVar");
    std::cout << "\n测试未定义变量错误:\n";
    vm2.registerGlobalFunction(undefVar);
    vm2.registerGlobalFunction(main);
    vm2.run();

    RainVirtualMachine vm3;
    main.instructions[0].operand = std::string("stackUnderflow");
    std::cout << "\n测试栈下溢错误:\n";
    vm3.registerGlobalFunction(stackUnderflow);
    vm3.registerGlobalFunction(main);
    vm3.run();
}

int main() {
    testVariableRegistration();
    testFunctionRegistration();
    testBasicOperations();
    testFunctionCall();
    testConditionals();
    testErrorHandling();

    std::cout << "\n所有测试完成!\n";
    return 0;
}