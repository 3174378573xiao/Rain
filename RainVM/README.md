# RainVM 虚拟机使用指南

## 概述

RainVM 是一个基于栈的虚拟机，支持多种数据类型和基本操作指令。它可以执行预定义的函数，支持变量存储、算术运算、逻辑运算、流程控制等功能。

## 核心特性

- **多数据类型支持**：int, double, string, bool
- **完整的指令集**：包含加载/存储、算术运算、逻辑运算、流程控制等指令
- **函数调用机制**：支持参数传递和返回值
- **变量作用域**：区分全局变量和局部变量
- **交互式输入**：支持从标准输入读取数据

## 数据类型

RainVM 支持以下数据类型：
- `int` - 整型
- `double` - 浮点型
- `string` - 字符串
- `bool` - 布尔值

## 指令集说明

### 基本操作指令
- `LOAD` - 将变量加载到栈顶
- `SAVE` - 将栈顶值保存到变量
- `PUSH` - 将值压入栈顶
- `POP` - 弹出栈顶值
- `PRINT` - 打印并弹出栈顶值
- `INPUT` - 从标准输入读取值并压入栈顶
- `DUP` - 复制栈顶值

### 算术运算指令
- `ADD` - 加法
- `SUB` - 减法
- `MUL` - 乘法
- `DIV` - 除法
- `MOD` - 取模
- `NEG` - 取负

### 逻辑运算指令
- `EQ` - 等于
- `NEQ` - 不等于
- `GT` - 大于
- `LT` - 小于
- `GTE` - 大于等于
- `LTE` - 小于等于
- `AND` - 逻辑与
- `OR` - 逻辑或
- `NOT` - 逻辑非

### 流程控制指令
- `JMP` - 无条件跳转
- `JMP_IF` - 条件为真跳转
- `JMP_NOT` - 条件为假跳转
- `CALL` - 函数调用
- `RET` - 函数返回
- `HALT` - 停止执行

## 使用方法

### 1. 初始化虚拟机

```cpp
RainVM vm;
```

### 2. 注册全局变量

```cpp
vm.registerGlobalVariable("variable_name", value);
```

示例：
```cpp
vm.registerGlobalVariable("END", 4); // 注册整型变量
vm.registerGlobalVariable("PI", 3.14159); // 注册浮点变量
vm.registerGlobalVariable("name", "RainVM"); // 注册字符串变量
vm.registerGlobalVariable("flag", true); // 注册布尔变量
```

### 3. 注册函数

函数由指令序列和参数列表组成：

```cpp
Function myFunction = {
    {指令1, 指令2, ...}, // 指令序列
    {"param1", "param2", ...} // 参数名列表
};

vm.registerGlobalFunction("function_name", myFunction);
```

示例：定义一个计算阶乘的函数
```cpp
Function Factorial = {
    {
        // 初始化变量
        {PUSH, 1},
        {SAVE, "I"},
        {PUSH, 1},
        {SAVE, "RESULT"},
        
        // 循环开始
        {LOAD, "I"},
        {LOAD, "END"},
        {LTE},
        {JMP_NOT, 17}, // 条件不满足时跳转到指令17
        
        // 计算 RESULT *= I
        {LOAD, "RESULT"},
        {LOAD, "I"},
        {MUL},
        {SAVE, "RESULT"},
        
        // I++
        {LOAD, "I"},
        {PUSH, 1},
        {ADD},
        {SAVE, "I"},
        
        // 跳回循环开始
        {JMP, 4},
        
        // 输出结果并返回
        {LOAD, "RESULT"},
        {PRINT},
        {RET}
    },
    {"END"} // 参数列表
};
```

### 4. 设置主函数

必须注册名为 "main" 的函数作为程序入口点：

```cpp
Function MAIN = {
    {
        // 主函数指令序列
        {PUSH, 5},
        {CALL, "Factorial"},
        {HALT}
    },
    {} // 无参数
};

vm.registerGlobalFunction("main", MAIN);
```

### 5. 运行虚拟机

```cpp
vm.run();
```

## 完整示例

```cpp
#include "RainVM.h"
using namespace std;

int main() {
    RainVM vm;
    
    // 定义阶乘函数
    Function Factorial = {
        {
            {PUSH, 1},
            {SAVE, "I"},
            {PUSH, 1},
            {SAVE, "RESULT"},
            
            {LOAD, "I"},
            {LOAD, "n"},
            {LTE},
            {JMP_NOT, 17},
            
            {LOAD, "RESULT"},
            {LOAD, "I"},
            {MUL},
            {SAVE, "RESULT"},
            
            {LOAD, "I"},
            {PUSH, 1},
            {ADD},
            {SAVE, "I"},
            
            {JMP, 4},
            {LOAD, "RESULT"},
            {PRINT},
            {RET}
        },
        {"n"}
    };
    
    // 定义主函数
    Function MAIN = {
        {
            {INPUT},        // 输入参数值
            {CALL, "Factorial"}, // 调用阶乘函数
            {HALT}            // 停止执行
        },
        {}
    };
    
    // 注册函数
    vm.registerGlobalFunction("Factorial", Factorial);
    vm.registerGlobalFunction("main", MAIN);
    
    // 运行虚拟机
    vm.run();
    
    return 0;
}
```

## 调试与查看状态

RainVM 提供了以下调试功能：

```cpp
// 查看全局变量表
vm.displayGlobalVariableTable();

// 查看全局函数表
vm.displayGlobalFunctionTable();
```

## 注意事项

1. 必须注册名为 "main" 的函数作为程序入口点
2. 变量名必须符合命名规范（以字母或下划线开头，后续字符可以是字母、数字或下划线）
3. 函数调用时，参数需要按照函数定义中的顺序压入栈中
4. 类型转换是隐式的，但不当的转换可能会导致运行时错误

## 错误处理

RainVM 会在遇到错误时输出错误信息，例如：
- 变量名不合法
- 变量已存在
- 函数未找到
- 类型转换错误
