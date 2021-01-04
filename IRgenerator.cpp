#include "SymbolList.h"


LLVMContext context;			
Type* myArray;								
IRBuilder<> builder(context);				
Module* module;							
Function* fun;			
Function* callFun;		
BasicBlock* bb;			
Value* icmp;			
BasicBlock* trueBB, * falseBB, * conti, * iterBB;
BasicBlock* retBB, *trueWhile, *falseWhile;
Value* additive;	
Value* mul, *br;	
Value* call;		
vector<Type*> functionParams;	
vector<string> functionParamsNames;	
vector<Value*> callFunParamsLoad;	
stack<BasicBlock*> trueBBforSelect, falseBBforSelect, contiBBForSelect, iterBBforWhile, trueBBforWhile, falseBBforWhile;


TreeNode* forGetCallPara;	
TreeNode* localDec, * stmt_list;	

string position = "global";				
map<variName, GlobalValue*> globalVariableAlloca;	
map<function_name, function_Info> functions;		
string temps;					
string funName;		

int lineno = 0;
FILE* source;		
FILE* listing;		
int TraceScan = FALSE;
int Error1 = FALSE;

void getIR(TreeNode* tree);			
						
function_Info newFunction() {
	function_Info t;		
	return t;
}
qtlocalVar newqtlocalVar() {	
	qtlocalVar t;
	return t;
}
vector<Value*> args;	

Value* arrayDec;
Type* arrayParam = ArrayType::get(Type::getInt32Ty(context), 0);	
Value* arrayTempEle;			
int arrayDecLength;				
forFunUsedInfo funUsedInfo;		
char program[100];			
NodeKind preNodeKind = errors; 

int nowptr;
int printTab(map<function_name, function_Info> functions);

raw_ostream *out = &outs();

int main() 
{
	TreeNode* syntaxTree;		
	strcpy(program, "code.c");
	source = fopen(program, "r");
	if (!source) 
	{
		fprintf(stderr, "Can not open the c\n");
		exit(-1);
	}
	module = new Module(program, context);
	listing = stdout;		
	syntaxTree = parse();	
	
	fprintf(listing, "\nSyntax tree:\n");
	printTree(syntaxTree);
	getIR(syntaxTree);
	printTab(functions);
	builder.ClearInsertionPoint();
	FILE* stream;
	stream = freopen("IRcode.ll", "w", stdout);
	module->print(outs(), nullptr);	
	delete module;
	return 0;
}


void getIR(TreeNode * tree) 
{
	
	for (; tree != NULL; tree = tree->sibling) 
	{
		switch (tree->nodeKind) 
		{
		case errors:
			preNodeKind = errors;
			break;
		case variabledeclaration:
			temps.clear();
			temps = tree->node.variable_definition.variable->node.ID;	
			if (position=="global") 
			{
				cout << "line: " << tree->lineno<<"全局变量为非法变量！" << endl;
			}
			else
			{
				nowptr = functions[position].localVarStack.size() - 1;  
				if (functions[position].localVarStack[nowptr].localVariableAlloca.count(temps) == 0)
				{
					
					functions[position].localVarStack[nowptr].localVariableAlloca[temps] = builder.CreateAlloca(Type::getInt32Ty(context));
					functions[position].localVarStack[nowptr].localVariableLoad[temps] = NULL;			
					
					functions[position].localVarInfo[temps].varName = temps;
					functions[position].localVarInfo[temps].decLineNo = tree->lineno;
					functions[position].localVarInfo[temps].type = MYINT;
					functions[position].localVarInfo[temps].isParam = false;
				
				}
				else 
				{
					cout << "line： " << tree->lineno << " 变量："<<temps<<"重复定义！" << endl;
				}
			}
			preNodeKind = variabledeclaration;
			break;

		case arraydeclaration:
			temps.clear();
			temps = tree->node.array_definition.variable->node.ID;	
			if (position == "global") 
			{
				cout << "line： " << tree->lineno << "全局变量为非法变量！" << endl;
			}
			else 
			{
				nowptr = functions[position].localVarStack.size() - 1;
				if( functions[position].localVarStack[nowptr].arrayAlloca.count(temps) == 0 )
				{
					arrayDecLength = tree->node.array_definition.length->node.NUM;		
					myArray = ArrayType::get(Type::getInt32Ty(context), arrayDecLength);			
					arrayDec = builder.CreateAlloca(myArray);		
					functions[position].localVarStack[nowptr].arrayAlloca[temps] = arrayDec;
					
					
					functions[position].localVarInfo[temps].varName = temps;
					functions[position].localVarInfo[temps].decLineNo = tree->lineno;
					functions[position].localVarInfo[temps].type = MYARRAY;
					functions[position].localVarInfo[temps].length = arrayDecLength;
					functions[position].localVarInfo[temps].isParam = false;
					/*-------------------------------------------------*/
				}
				else
				{
					cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
				}
			}
			break;

		case functiondeclaration:
			funName = tree->node.func_definition.variable->node.ID; 
			functions[funName] = newFunction();			
			functions[funName].funcDecLineNo = tree->lineno;
			functionParams.clear();
			functionParamsNames.clear();
			position = funName;
			functions[position].localVarStack.push_back(newqtlocalVar());	
			getIR(tree->node.func_definition.parameters);			
			if (tree->node.func_definition.type_specifier->node.tokentype == INT) 
			{		
				fun = Function::Create(FunctionType::get(Type::getInt32Ty(context), functionParams, false),
					GlobalValue::LinkageTypes::ExternalLinkage,
					funName, module);
			}
			else if (tree->node.func_definition.type_specifier->node.tokentype == VOID) 
			{
				fun = Function::Create(FunctionType::get(Type::getVoidTy(context), functionParams, false),
					GlobalValue::LinkageTypes::ExternalLinkage,
					funName, module);
			}
			functions[position].function = fun;
			bb = BasicBlock::Create(context, "", fun);		
			builder.SetInsertPoint(bb);						
			nowptr = functions[position].localVarStack.size() - 1;
			for (int i = 0; i < functionParams.size(); i++)
			{
				if (functionParams[i] == Type::getInt32Ty(context)) 
				{
					functions[position].localVarStack[nowptr].localVariableAlloca[functionParamsNames[i]] = builder.CreateAlloca(Type::getInt32Ty(context));
				}
			}
			
			args.clear();
			for (auto arg = fun->arg_begin(); arg != fun->arg_end(); arg++) 
			{
				args.push_back(arg);
			}
			for (int i = 0; i < args.size(); i++) 
			{
				if (functionParams[i] == Type::getInt32Ty(context)) 
				{
					builder.CreateStore(args[i],
						functions[position].localVarStack[nowptr].localVariableAlloca[functionParamsNames[i]]);
				}
			}
			getIR(tree->node.func_definition.compound_sentence);
		
			
			functions[position].localVarStack.pop_back();	
			position = "global";		
			break;

		case variableparameter:
			temps = tree->node.variable_parameter.variable->node.ID;	
			nowptr = functions[position].localVarStack.size() - 1;
			if(functions[position].localVarStack[nowptr].localVariableAlloca.count(temps)==0){
				functionParams.push_back(Type::getInt32Ty(context));			
				functionParamsNames.push_back(temps);	
				
				functions[position].localVarInfo[temps].decLineNo = tree->lineno;
				functions[position].localVarInfo[temps].isParam = true;
				functions[position].localVarInfo[temps].type = MYINT;
				functions[position].localVarInfo[temps].varName = temps;
				
			}
			else {
				cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
			}
			break;

		case arrayparameter:
			temps = tree->node.variable_parameter.variable->node.ID;
			nowptr = functions[position].localVarStack.size() - 1;
			if (functions[position].localVarStack[nowptr].arrayAlloca.count(temps) == 0) {
				functionParams.push_back(arrayParam);
				functionParamsNames.push_back(temps);
				
				functions[position].localVarInfo[temps].decLineNo = tree->lineno;
				functions[position].localVarInfo[temps].isParam = true;
				functions[position].localVarInfo[temps].type = MYARRAY;
				functions[position].localVarInfo[temps].varName = temps;
				
			}
			else {
				cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
			}
			break;

		case compoundsentence:
			functions[position].localVarStack.push_back(newqtlocalVar());
			getIR(tree->node.compound_sentence.local_declarations);
			getIR(tree->node.compound_sentence.sentences);
			functions[position].localVarStack.pop_back();	
			break;

		case expressionsentence:
			getIR(tree->node.expression_sentence.expression);
			tree->Load = tree->node.expression_sentence.expression->Load;		
			break;

		case ifelsesentence:
			trueBB = BasicBlock::Create(context, "", fun);
			trueBBforSelect.push(trueBB);
			falseBB = BasicBlock::Create(context, "", fun);
			falseBBforSelect.push(falseBB);
			conti = BasicBlock::Create(context, "", fun);
			contiBBForSelect.push(conti);			
			getIR(tree->node.ifelse_sentense.expression);		
			if (tree->node.ifelse_sentense.expression->nodeKind == relationexpression)
			{
				icmp = tree->node.ifelse_sentense.expression->Load;
			}
			else 
			{
				icmp = builder.CreateICmpNE(tree->node.ifelse_sentense.expression->Load, ConstantInt::get(context, APInt(32, 0)));
			}
			trueBB = trueBBforSelect.top(); trueBBforSelect.pop();
			falseBB = falseBBforSelect.top(); 
			br = builder.CreateCondBr(icmp, trueBB, falseBB);
			builder.SetInsertPoint(trueBB);			
			getIR(tree->node.ifelse_sentense.if_sentence);
			if (preNodeKind != returnsentence) 
			{
				conti = contiBBForSelect.top();
				builder.CreateBr(conti);
			}
			falseBB = falseBBforSelect.top();	
			falseBBforSelect.pop();
			builder.SetInsertPoint(falseBB);
			
			getIR(tree->node.ifelse_sentense.else_sentence);
			if (preNodeKind != returnsentence)	
			{
				conti = contiBBForSelect.top();
				builder.CreateBr(conti);
			}
			conti = contiBBForSelect.top();  
			contiBBForSelect.pop();
			builder.SetInsertPoint(conti);
			break;

		case whilesentence:
			iterBB = BasicBlock::Create(context, "", fun);
			iterBBforWhile.push(iterBB);
			builder.CreateBr(iterBB);
			builder.SetInsertPoint(iterBB);
			getIR(tree->node.while_sentence.expression);
			if (tree->node.while_sentence.expression->nodeKind == relationexpression)
			{
				icmp = tree->node.while_sentence.expression->Load;
			}
			else 
			{
				icmp = builder.CreateICmpNE(tree->node.while_sentence.expression->Load, ConstantInt::get(context, APInt(32, 0)));
			}
			trueWhile = BasicBlock::Create(context, "", fun);
			
			falseWhile = BasicBlock::Create(context, "", fun);
			falseBBforWhile.push(falseWhile);
						
			br = builder.CreateCondBr(icmp, trueWhile, falseWhile);
			builder.SetInsertPoint(trueWhile);
			getIR(tree->node.while_sentence.loop_sentence);
			iterBB = iterBBforWhile.top();  
			iterBBforWhile.pop();
			builder.CreateBr(iterBB);
			falseWhile = falseBBforWhile.top();	
			falseBBforWhile.pop();
			builder.SetInsertPoint(falseWhile);
			break;

		case returnsentence:
			if (tree->node.return_sentence.return_expression != NULL)
			{
				getIR(tree->node.return_sentence.return_expression);
				
				builder.CreateRet(tree->node.return_sentence.return_expression->Load);
			}
			else 
			{
				builder.CreateRetVoid();
			}
			preNodeKind = returnsentence;
			break;

		case assignexpression:
			nowptr = functions[position].localVarStack.size() - 1;
			if (tree->node.assign_sentence.variable->nodeKind == IDs) 
			{
				getIR(tree->node.assign_sentence.expression);
				bool flag = 0;
				temps = tree->node.assign_sentence.variable->node.ID;
				for (int i = nowptr; i >= 0; i--) 
				{
					if (functions[position].localVarStack[i].localVariableAlloca.count(temps) != 0) {
						builder.CreateStore(builder.CreateLoad(tree->node.assign_sentence.expression->Alloca),
											functions[position].localVarStack[i].localVariableAlloca[temps]);
						functions[position].localVarInfo[temps].useLineNo.push_back(tree->lineno);
						
						flag = 1;
						break;
					}
				}
				
				if (flag == 0) {
					cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
				}
			}
			else if (tree->node.assign_sentence.variable->nodeKind == arrays)	
			{
				int flag = 0;
				getIR(tree->node.assign_sentence.expression);
				getIR(tree->node.assign_sentence.variable->node.array.arr_expression);
				temps = tree->node.assign_sentence.variable->node.array.arraynode->node.ID;
				for (int i = nowptr; i >= 0; i--) {			
					if (functions[position].localVarStack[i].arrayAlloca.count(temps) != 0) {
						arrayTempEle = builder.CreateGEP(functions[position].localVarStack[i].arrayAlloca[temps],
							{tree->node.assign_sentence.variable->node.array.arr_expression->Load });	
						builder.CreateStore(tree->node.assign_sentence.expression->Load, arrayTempEle);	
						flag = 1;
						functions[position].localVarInfo[temps].useLineNo.push_back(tree->lineno);
						break;
					}
				}
				if (flag == 0) {
					cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
				}
			}
			break;

		case relationexpression:
			getIR(tree->node.relation_op.left_expression);
			getIR(tree->node.relation_op.right_expression);
			
			
			switch (tree->node.relation_op.operation->node.tokentype)	
			{
			case LT:icmp = builder.CreateICmpSLT(tree->node.relation_op.left_expression->Load, 
				tree->node.relation_op.right_expression->Load); 
				break;
			case LE:icmp = builder.CreateICmpSLE(tree->node.relation_op.left_expression->Load,
				tree->node.relation_op.right_expression->Load); 
				break;
			case GT:icmp = builder.CreateICmpSGT(tree->node.relation_op.left_expression->Load, 
				tree->node.relation_op.right_expression->Load); 
				break;
			case GE:icmp = builder.CreateICmpSGE(tree->node.relation_op.left_expression->Load, 
				tree->node.relation_op.right_expression->Load); 
				break;
			case EQ:icmp = builder.CreateICmpEQ(tree->node.relation_op.left_expression->Load, 
				tree->node.relation_op.right_expression->Load);
				break;
			case NE:icmp = builder.CreateICmpNE(tree->node.relation_op.left_expression->Load,
				tree->node.relation_op.right_expression->Load); 
				break;
			default:
				break;
			}
			tree->Load = icmp;	
			break;

		case addsubexpression:
			getIR(tree->node.add_sub_op.left_expression);	
			getIR(tree->node.add_sub_op.right_expression);	
			switch (tree->node.add_sub_op.operation->node.tokentype)
			{
			case PLUS:	additive = builder.CreateNSWAdd(tree->node.add_sub_op.left_expression->Load,
				tree->node.add_sub_op.right_expression->Load); 
				break;
			
			case MINUS:	additive = builder.CreateNSWSub(tree->node.add_sub_op.left_expression->Load,
				tree->node.add_sub_op.right_expression->Load); 
				break;
			default:
				break;
			}
			if (tree->Alloca == NULL) 
			{
				tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
			}
			builder.CreateStore(additive, tree->Alloca);
			tree->Load = builder.CreateLoad(tree->Alloca);
			break;


		case muldivexpression:		
			getIR(tree->node.mul_div_op.left_expression);
			getIR(tree->node.mul_div_op.right_expression);
			switch (tree->node.mul_div_op.operation->node.tokentype)
			{
			case TIMES:	additive = builder.CreateNSWMul(tree->node.mul_div_op.left_expression->Load,
				tree->node.mul_div_op.right_expression->Load); 
				break;
			case OVER:	additive = builder.CreateSDiv(tree->node.mul_div_op.left_expression->Load, 
				tree->node.mul_div_op.right_expression->Load); 
				break;
			default:
				break;
			}
			if (tree->Alloca == NULL)
			{
				tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
			}
			builder.CreateStore(additive, tree->Alloca);
			tree->Load = builder.CreateLoad(tree->Alloca);
			break;

		case IDs:
		{
			temps = tree->node.ID;	
			int flag = 0;
			nowptr = functions[position].localVarStack.size() - 1;
			for (int i = nowptr; i >= 0; i--) 
			{
				if (functions[position].localVarStack[i].localVariableAlloca.count(temps) != 0) 
				{
					if (tree->Alloca == NULL)
					{
						tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
					}
					builder.CreateStore(builder.CreateLoad(functions[position].localVarStack[i].localVariableAlloca[temps]), tree->Alloca);
					tree->Load = builder.CreateLoad(tree->Alloca);
					/*=========================  for symbol table  =============================*/
					functions[position].localVarInfo[temps].useLineNo.push_back(tree->lineno);
					/*-------------------------------------------------------------------------------*/
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				cout << "line： " << tree->lineno << " 变量：" << temps << "重复定义：" << endl;
			}
			break;
		}
		case arrays:		
		{

			int flag = 0;
			if (tree->Alloca == NULL)
			{
				tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
			}
			getIR(tree->node.array.arr_expression);
			temps = tree->node.array.arraynode->node.ID;
			nowptr = functions[position].localVarStack.size() - 1;
			for (int i = nowptr; i >= 0; i--) 
			{
				if (functions[position].localVarStack[i].arrayAlloca.count(temps) != 0) 
				{
					arrayTempEle = builder.CreateGEP(functions[position].localVarStack[i].arrayAlloca[temps],
						{tree->node.array.arr_expression->Load });
					builder.CreateStore(builder.CreateLoad(arrayTempEle), tree->Alloca);
					tree->Load = builder.CreateLoad(tree->Alloca);
					/*-----for symbol table -------------*/
					functions[position].localVarInfo[temps].useLineNo.push_back(tree->lineno);
					/*--------------------------------------------------------------------------*/
					flag = 1;
				}
			}
			if (flag == 0) {
				cout << "line: " << tree->lineno << " 变量：" << temps << "重复定义！" << endl;
			}
			break;
		}

		case calls:
			temps = tree->node.call.variable->node.ID;
			if (functions.count(temps) == 0) 
			{
				cout << "ERROR! 函数 \"" << temps << "\" 未定义！" << endl;
			}
			else 
			{
				callFun = functions[temps].function;
				callFunParamsLoad.clear();
				getIR(tree->node.call.expression_list);
				forGetCallPara = tree->node.call.expression_list;
				while (forGetCallPara)
				{
					callFunParamsLoad.push_back(forGetCallPara->Load);
					forGetCallPara = forGetCallPara->sibling;
				}
				
				call = builder.CreateCall(callFun, callFunParamsLoad);
				if (tree->Alloca == NULL)
				{
					tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
				}
				builder.CreateStore(call, tree->Alloca);
				tree->Load = builder.CreateLoad(tree->Alloca);
				/*===============================symbol table============================*/
				functions[position].used_funName.push_back(temps);
				funUsedInfo.caller = position;
				funUsedInfo.lineno = tree->lineno;
				functions[temps].beUsed.push_back(funUsedInfo);
				/*=================================================================*/
			}
			break;

		case NUMs:
			if (tree->Alloca == NULL)
			{
				tree->Alloca = builder.CreateAlloca(Type::getInt32Ty(context));
			}
			builder.CreateStore(ConstantInt::get(context, APInt(32, tree->node.NUM)), tree->Alloca);
			tree->Load = builder.CreateLoad(tree->Alloca);
			break;

		case tokentypes:
			break;
		default:
			;
		}
	}
	
}
