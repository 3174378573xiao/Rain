#include "RainVM.h"
using namespace std;


int main() {
	RainVM vm;
	Function Factorial = {
		{
			//INT I =0 ;RESLUT =0
		{PUSH,1},
		{SAVE,"I"},
		{PUSH,1},
		{SAVE,"RESLUT"},

		//IF
		{LOAD,"I"},
		{LOAD,"END"},
		{LTE},
		{JMP_NOT,17},

		//RESLUT*=I
		{LOAD,"RESLUT"},
		{LOAD,"I"},
		{MUL},
		{SAVE,"RESLUT"},

		//I++
		{LOAD,"I"},
		{PUSH,1},
		{ADD},
		{SAVE,"I"},

		//
		{JMP,4},
		{LOAD,"RESLUT"},
		{PRINT},
		{RET},
		},
		{"END"},
	};
	//Function test = {
	//	{
	//	{LOAD,"a"},
	//	{PRINT},
	//	{RET},
	//	},
	//	{"a"}
	//};
	Function test = {
		{
		{LOAD,"a"},
		{PRINT},
		{LOAD,"b"},
		{PRINT},
		{LOAD,"c"},
		{PRINT},
		},
		{"a","b","c"}
	};

	Function MAIN = {
		{
		//{PUSH,4},
		//{CALL,"Factorial"},
		//{PUSH,"\nOVER!\n"},
		//{PRINT}

		//{PUSH,"a"},
		//{PUSH,"b"},
		//{PUSH,"c"},
		//{CALL,"test"},

			{INPUT},
		{PRINT},
		},
	};
	vm.registerGlobalFunction("main", MAIN);
	//vm.registerGlobalFunction("TEST", test);
	//vm.registerGlobalFunction("main", Factorial);
	vm.registerGlobalFunction("Factorial", Factorial);
	vm.registerGlobalFunction("test", test);
	//vm.registerGlobalVariable("END", 4);
	//vm.displayGlobalVariableTable();
	vm.run();

	return 0;
}