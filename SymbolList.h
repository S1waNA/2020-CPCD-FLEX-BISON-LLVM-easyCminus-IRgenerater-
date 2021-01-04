#include "treenodeAPI.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
#include <map>
#include<stack>
#include<fstream>
#include <memory>
#include <string>
#include<iomanip>

using namespace std;

using namespace llvm;
typedef string function_name;
typedef string variName;
typedef string arrName;

#define MYINT 1
#define MYARRAY 2

struct VarInfo {
	string varName;
	int decLineNo;
	vector<int> useLineNo;
	bool isParam;
	int type;	
	int length;	
};

struct forFunUsedInfo {
	string caller;
	int lineno;
};
struct qtlocalVar {
	map<variName, Value*> localVariableAlloca;
	map<variName, Value*> localVariableLoad;
	map<arrName, Value*> arrayAlloca;
};
struct function_Info {
	int funcDecLineNo;
	vector<qtlocalVar> localVarStack;
	vector<string>  used_funName; 
	vector<forFunUsedInfo> beUsed;
	map<variName, VarInfo>  localVarInfo;

	Function* function;
}; 

#define MAXTOKENLEN 40
extern char tokenString[MAXTOKENLEN+1];

TreeNode *parse(void);

TokenType getToken(void);