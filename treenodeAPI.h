#ifndef _treenodeAPI_H_
#define _treenodeAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
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
#include <memory>
#include <string>
using namespace std;
using namespace llvm;

#ifndef YYPARSER
#include "cm.tab.h"
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 8
#define DONT_OCCUR_PRINT

//AST数据结构

typedef int TokenType;

extern FILE* source;
extern FILE* listing;
extern FILE* code;
extern int lineno; 

typedef enum {
    errors,

    variabledeclaration,
    arraydeclaration,
    functiondeclaration,

    variableparameter,
    arrayparameter,

    compoundsentence,
    expressionsentence,
    ifelsesentence,
    whilesentence,
    returnsentence,

    assignexpression,
    relationexpression,
    addsubexpression,
    muldivexpression,

    IDs,
    arrays,
    calls,

    NUMs,
    tokentypes,
} NodeKind;

typedef enum {
    NotResolvedT,
    ErrorT,
    NoneT,
    VoidT,
    IntT,
    IntArrayT,
    FuncT,
} ExpType;

typedef struct
{
  ExpType nodeType;
  union {
     
      struct {
          int isParam;
          int memloc; 
          int globalFlag;
      } intInfo;

      struct {
          int isParam;
          int memloc;
          int globalFlag;
          int arrLen;
      } arrInfo;

      struct {
          ExpType retType;
          int paramLen;
          ExpType * paramTypeList;
      } funcInfo;
  } attr;
} SymbolInfo;

typedef struct treeNode {
  struct treeNode *sibling;
  int lineno;
  NodeKind nodeKind;
  ExpType nodeType;
  SymbolInfo * symbolInfo;
  Value* Alloca;
  Value* Load;
  union {

      struct {
          struct treeNode *type_specifier;
          struct treeNode *variable;
      } variable_definition;


      struct {
          struct treeNode *type_specifier;
          struct treeNode *variable;
          struct treeNode *length;
      } array_definition;

      struct {
          struct treeNode *type_specifier;
          struct treeNode *variable;
          struct treeNode *parameters;
          struct treeNode *compound_sentence;
		  BasicBlock* funEntry;
      } func_definition;

      struct {
          struct treeNode *type_specifier;
          struct treeNode *variable;
      } variable_parameter;

      struct {
          struct treeNode *type_specifier;
          struct treeNode *arraynode;
      } array_parameter;
      
      struct {
          struct treeNode *local_declarations;
          struct treeNode *sentences;
        
      } compound_sentence;

      struct {
          struct treeNode *expression;

      } expression_sentence;


      struct {
          struct treeNode *expression;
          struct treeNode *if_sentence;
          struct treeNode *else_sentence;
      } ifelse_sentense;

      struct {
          struct treeNode *expression;
          struct treeNode *loop_sentence;
      } while_sentence;

   
      struct {
          struct treeNode *return_expression;

      } return_sentence;

      struct {
          struct treeNode *expression;
          struct treeNode *variable;
      } assign_sentence;

      struct {
          struct treeNode *left_expression;
          struct treeNode *operation;
          struct treeNode *right_expression;
	
      } relation_op;

   
      struct {
          struct treeNode *left_expression;
          struct treeNode *operation;
          struct treeNode *right_expression;

      } add_sub_op;

  
      struct {
          struct treeNode *left_expression;
          struct treeNode *operation;
          struct treeNode *right_expression;
	
      } mul_div_op;

 
      struct {
          struct treeNode *arraynode;
          struct treeNode *arr_expression;
      } array;

      struct {
          struct treeNode *variable;
          struct treeNode *expression_list;
	
      } call;

   
      struct {
          char *ID;
	
      };

   
      struct {
          int NUM;

      };

 
      struct {
          TokenType tokentype;
      };
  } node;
} TreeNode;

extern int EchoSource;

extern int TraceScan;

extern int TraceParse;

extern int TraceAnalyze;

extern int TraceCode;

extern int Error1; 



//AST函数

TreeNode* addSibling(TreeNode *, TreeNode *);
TreeNode* create_tree_node();
int TokenTypeChecker(TokenType);
int NodeKindChecker(TreeNode *, NodeKind);

TreeNode *create_variable_node(TreeNode *, TreeNode *);
TreeNode *create_array_node(TreeNode *, TreeNode *, TreeNode *);
TreeNode *create_func_node(TreeNode *, TreeNode *, TreeNode *, TreeNode *);

TreeNode *create_variable_param(TreeNode *, TreeNode *);
TreeNode *create_array_param(TreeNode *, TreeNode *);

TreeNode *create_compound_sentence(TreeNode *, TreeNode *);
TreeNode *create_expression_sentence(TreeNode *);
TreeNode *create_ifelse_sentence(TreeNode *, TreeNode *, TreeNode *);
TreeNode *create_while_sentence(TreeNode *, TreeNode *);
TreeNode *create_return_sentence(TreeNode *);

TreeNode *create_assign_sentence(TreeNode *, TreeNode *);
TreeNode *create_relationop_sentence(TreeNode *, TreeNode *, TreeNode *);
TreeNode *create_addsubop_sentence(TreeNode *, TreeNode *, TreeNode *);
TreeNode *create_muldivop_sentence(TreeNode *, TreeNode *, TreeNode *);

TreeNode *create_ID_node(char *);
TreeNode *create_call_sentence(TreeNode *, TreeNode *);
TreeNode *create_array_sentence(TreeNode *, TreeNode *);
TreeNode *create_NUM_node(char *);
TreeNode *create_token_node(TokenType);

void printToken(TokenType, const char*);

char* copyString(char*);

void printTree(TreeNode*);

#endif
