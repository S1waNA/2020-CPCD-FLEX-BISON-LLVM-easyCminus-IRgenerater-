#include "treenodeAPI.h"
#include "SymbolList.h"


TreeNode*addSibling(TreeNode *origin, TreeNode *follow)
{
  if (origin != NULL) {
    TreeNode *t = origin;
    while (t->sibling != NULL) t = t->sibling;
    t->sibling = follow;
  }
  else {
    origin = follow;
  }
  return origin;
}

TreeNode*create_tree_node()
{
  TreeNode *t = (TreeNode*)malloc(sizeof(TreeNode));
  if (t == NULL)
    {
      fprintf(listing,
              "Out of memory error at line %d\n",
              lineno);
    }
  else
    {
      t->sibling = NULL;
      t->lineno = lineno;
	  t->Alloca = NULL;
	  t->Load = NULL;
    }
  return t;
}

TreeNode*create_variable_node(TreeNode *type_specifier,TreeNode *variable)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = variabledeclaration;
      t->node.variable_definition.type_specifier = type_specifier;
      t->node.variable_definition.variable = variable;
    }
  return t;
}

TreeNode*create_array_node(TreeNode *type_specifier,TreeNode *variable,TreeNode *length)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = arraydeclaration;
      t->node.array_definition.type_specifier = type_specifier;
      t->node.array_definition.variable = variable;
      t->node.array_definition.length = length;

    }
  return t;
}

TreeNode*create_func_node(TreeNode *type_specifier,TreeNode *variable,TreeNode *parameters,TreeNode *compound_sentence)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = functiondeclaration;
      t->node.func_definition.type_specifier = type_specifier;
      t->node.func_definition.variable = variable;
      t->node.func_definition.parameters = parameters;
      t->node.func_definition.compound_sentence = compound_sentence;
    }
  return t;
}

TreeNode*create_variable_param(TreeNode *type_specifier,TreeNode *variable)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = variableparameter;
      t->node.variable_parameter.type_specifier = type_specifier;
      t->node.variable_parameter.variable = variable;
    }

  return t;
}

TreeNode*create_array_param(TreeNode *type_specifier,TreeNode *arraynode)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = arrayparameter;
      t->node.array_parameter.type_specifier = type_specifier;
      t->node.array_parameter.arraynode = arraynode;
    }
  return t;
}

TreeNode*create_compound_sentence(TreeNode *local_declarations, TreeNode *sentences)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = compoundsentence;
      t->node.compound_sentence.local_declarations = local_declarations;
      t->node.compound_sentence.sentences = sentences;
    }
  return t;
}

TreeNode*create_expression_sentence(TreeNode *expression) 
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = expressionsentence;
      t->node.expression_sentence.expression = expression;
    }
  return t;
}

TreeNode*create_ifelse_sentence(TreeNode *expression, TreeNode *if_sentence,TreeNode *else_sentence) 
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = ifelsesentence;
      t->node.ifelse_sentense.expression = expression;
      t->node.ifelse_sentense.if_sentence = if_sentence;
      t->node.ifelse_sentense.else_sentence = else_sentence;
    }
  return t;
}

TreeNode*create_while_sentence(TreeNode *expression,TreeNode *loop_sentence) 
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = whilesentence;
      t->node.while_sentence.expression = expression;
      t->node.while_sentence.loop_sentence = loop_sentence;
    }
  return t;
}

TreeNode*create_return_sentence(TreeNode *return_expression) 
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = returnsentence;
      t->node.return_sentence.return_expression = return_expression;
    }
  return t;
}

TreeNode*create_assign_sentence(TreeNode *variable,TreeNode *expression)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = assignexpression;
      t->node.assign_sentence.variable = variable;
      t->node.assign_sentence.expression = expression;
    }
  return t;
}

TreeNode*create_relationop_sentence(TreeNode *left_expression,TreeNode *operation,TreeNode *right_expression)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = relationexpression;
      t->node.relation_op.left_expression = left_expression;
      t->node.relation_op.operation = operation;
      t->node.relation_op.right_expression = right_expression;
    }
  return t;
}

TreeNode*create_addsubop_sentence(TreeNode *left_expression,TreeNode *operation,TreeNode *right_expression)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = addsubexpression;
      t->node.add_sub_op.left_expression = left_expression;
      t->node.add_sub_op.operation = operation;
      t->node.add_sub_op.right_expression = right_expression;
    }
  return t;
}

TreeNode*create_muldivop_sentence(TreeNode *left_expression,TreeNode *operation,TreeNode *right_expression)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = muldivexpression;
      t->node.mul_div_op.left_expression = left_expression;
      t->node.mul_div_op.operation = operation;
      t->node.mul_div_op.right_expression = right_expression;
    }
  return t;
}

TreeNode*create_array_sentence(TreeNode *arraynode,TreeNode *arr_expression)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = arrays;
      t->node.array.arraynode = arraynode;
      t->node.array.arr_expression = arr_expression;
    }
  return t;
}

TreeNode*create_call_sentence(TreeNode *variable,TreeNode *expression_list)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = calls;
      t->node.call.variable = variable;
      t->node.call.expression_list = expression_list;
    }
  return t;
}

TreeNode*create_ID_node(char *ID)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = IDs;
      t->node.ID = copyString(ID);
    }
  return t;
}

TreeNode*create_NUM_node(char *NUM)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = NUMs;
      t->node.NUM = atoi(NUM);
    }
  return t;
}

TreeNode*create_token_node(TokenType tokentype)
{
  TreeNode * t = create_tree_node();
  if (t != NULL)
    {
      t->nodeKind = tokentypes;
      t->node.tokentype = tokentype;
    }
  return t;
}


#define MALLOC(ptr, size) do { \
    ptr = (char*)malloc(size);\
    if(ptr == NULL) {\
        fprintf(listing, "%s:%d:%s: Memory allocation failed.\n", \
                __FILE__, __LINE__, __FUNCTION__); \
        assert(0);\
    }\
} while(0)
char*copyString(char * s)
{
  size_t n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  MALLOC(t, n);
  strcpy(t,s);
  return t;
}

static int indentno = 0;

#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void)
{
  int i;
  for (i=0; i<indentno; i++) fprintf(listing," ");
}


#define PRINTDESC(...)\
  do {\
      printSpaces();\
      fprintf(listing, __VA_ARGS__);\
  } while(0);
static const char *operatorString(TokenType op)
{
  if(op == INT) return "int";
  if(op == VOID) return "void";

  if(op == PLUS) return "+";
  if(op == MINUS) return "-";
  if(op == TIMES) return "*";
  if(op == OVER) return "/";

  if(op == LT) return "<";
  if(op == LE) return "<=";
  if(op == GT) return ">";
  if(op == GE) return ">=";
  if(op == EQ) return "==";
  if(op == NE) return "!=";

  return "";
}
void printTree(TreeNode* tree)
{
  INDENT;
  if (tree == NULL) PRINTDESC("(null)\n");
  for (; tree != NULL; tree = tree->sibling)
    {
      switch (tree->nodeKind)
        {
        case errors:
          PRINTDESC("[DEBUG] ErrorK at printTree\n");
          break;

        case variabledeclaration:
          PRINTDESC("Variable Declaration\n");
          printTree(tree->node.variable_definition.type_specifier);
          printTree(tree->node.variable_definition.variable);
          break;

        case arraydeclaration:
          PRINTDESC("Array Declaration\n");
          printTree(tree->node.array_definition.type_specifier);
          printTree(tree->node.array_definition.variable);
          printTree(tree->node.array_definition.length);
          break;

        case functiondeclaration:
          PRINTDESC("Function Declaration\n");
          printTree(tree->node.func_definition.type_specifier);
          printTree(tree->node.func_definition.variable);
          PRINTDESC("> Parameters :\n");
          printTree(tree->node.func_definition.parameters);
          PRINTDESC("> Function Block :\n");
          printTree(tree->node.func_definition.compound_sentence);
          break;

        case variableparameter:
          PRINTDESC("Parameter (Variable)\n");
          printTree(tree->node.variable_parameter.type_specifier);
          printTree(tree->node.variable_parameter.variable);
          break;

        case arrayparameter:
          PRINTDESC("Parameter (Array)\n");
          printTree(tree->node.array_parameter.type_specifier);
          printTree(tree->node.array_parameter.arraynode);
          break;

        case compoundsentence:
          PRINTDESC("Compound Statement\n");
          PRINTDESC("> Local Declarations :\n");
          printTree(tree->node.compound_sentence.local_declarations);
          PRINTDESC("> Local Statements :\n");
          printTree(tree->node.compound_sentence.sentences);
          break;

        case expressionsentence:
          PRINTDESC("Expression Statement\n");
          PRINTDESC("> Expression :\n");
          printTree(tree->node.expression_sentence.expression);
          break;

        case ifelsesentence:
          PRINTDESC("Selection Statement\n");
          PRINTDESC("> Expression inside if(*) :\n");
          printTree(tree->node.ifelse_sentense.expression);
          PRINTDESC("> Statements inside if clause :\n");
          printTree(tree->node.ifelse_sentense.if_sentence);
          PRINTDESC("> Statements inside else clause :\n");
          printTree(tree->node.ifelse_sentense.else_sentence);
          break;

        case whilesentence:
          PRINTDESC("Iteration Statement\n");
          PRINTDESC("> Expression inside while(*) :\n");
          printTree(tree->node.while_sentence.expression);
          PRINTDESC("> Statements inside while clause :\n");
          printTree(tree->node.while_sentence.loop_sentence);
          break;

        case returnsentence:
          PRINTDESC("Return Statement\n");
          PRINTDESC("> Returning expression :\n");
          printTree(tree->node.return_sentence.return_expression);
          break;

        case assignexpression:
          PRINTDESC("Assignment Expression\n");
          PRINTDESC("> Variable associated to assignment :\n");
          printTree(tree->node.assign_sentence.variable);
          PRINTDESC("> Value assigned :\n");
          printTree(tree->node.assign_sentence.expression);
          break;

        case relationexpression:
          PRINTDESC("Comparison Expression\n");
          printTree(tree->node.relation_op.operation);
          PRINTDESC("> Left expression compared :\n");
          printTree(tree->node.relation_op.left_expression);
          PRINTDESC("> Right expression compared :\n");
          printTree(tree->node.relation_op.right_expression);
          break;

        case addsubexpression:
          PRINTDESC("Additive Expression\n");
          printTree(tree->node.add_sub_op.operation);
          PRINTDESC("> Left expression added / subtracted :\n");
          printTree(tree->node.add_sub_op.left_expression);
          PRINTDESC("> Right expression added / subtracted :\n");
          printTree(tree->node.add_sub_op.right_expression);
          break;

        case muldivexpression:
          PRINTDESC("Multiplicative Expression\n");
          printTree(tree->node.mul_div_op.operation);
          PRINTDESC("> Left expression multiplied / divided :\n");
          printTree(tree->node.mul_div_op.left_expression);
          PRINTDESC("> Right expression multiplied / divided :\n");
          printTree(tree->node.mul_div_op.right_expression);
          break;

        case IDs:
          PRINTDESC("Variable Id : %s\n", tree->node.ID);
          break;

        case arrays:
          PRINTDESC("Array\n");
          printTree(tree->node.array.arraynode);
          PRINTDESC("> Expression inside subscript [*]\n");
          printTree(tree->node.array.arr_expression);
          break;

        case calls:
          PRINTDESC("Function Call\n");
          printTree(tree->node.call.variable);
          PRINTDESC("> Function arguments :\n");
          printTree(tree->node.call.expression_list);
          break;

        case NUMs:
          PRINTDESC("Constant : %d\n", tree->node.NUM);
          break;

        case tokentypes:
          PRINTDESC("Token : %s\n", operatorString(tree->node.tokentype));
          break;

        default:
          PRINTDESC("[DEBUG] No such nodeKind\n");
        }
    }
  UNINDENT;
}

void printToken(TokenType token, const char* tokenString)
{
  switch (token)
    {
    case ENDFILE: 
      fprintf(listing,"EOF\n"); break;
    case ERROR:   
      fprintf(listing,"ERROR\t\t\t%s\n",tokenString);
      break;
    case ID:      
      fprintf(listing,"ID\t\t\t%s\n",tokenString);
      break;
    case NUM:     
      fprintf(listing,"NUM\t\t\t%s\n",tokenString);
      break;
    case ELSE:    
      fprintf(listing,"ELSE\t\t\t%s\n",tokenString);
      break;
    case IF:      
      fprintf(listing,"IF\t\t\t%s\n",tokenString);
      break;
    case INT:     
      fprintf(listing,"INT\t\t\t%s\n",tokenString);
      break;
    case RETURN:  
      fprintf(listing,"RETURN\t\t\t%s\n",tokenString);
      break;
    case VOID:    
      fprintf(listing,"VOID\t\t\t%s\n",tokenString);
      break;
    case WHILE:   
      fprintf(listing,"WHILE\t\t\t%s\n",tokenString);
      break;
    case PLUS:    
      fprintf(listing,"PLUS\t\t\t%s\n",tokenString);
      break;
    case MINUS:   
      fprintf(listing,"MINUS\t\t\t%s\n",tokenString);
      break;
    case TIMES:   
      fprintf(listing,"TIMES\t\t\t%s\n",tokenString);
      break;
    case OVER:    
      fprintf(listing,"OVER\t\t\t%s\n",tokenString);
      break;
    case LT:      
      fprintf(listing,"<\t\t\t%s\n",tokenString);
      break;
    case LE:      
      fprintf(listing,"<=\t\t\t%s\n",tokenString);
      break;
    case GT:      
      fprintf(listing,">\t\t\t%s\n",tokenString);
      break;
    case GE:      
      fprintf(listing,">=\t\t\t%s\n",tokenString);
      break;
    case EQ:      
      fprintf(listing,"==\t\t\t%s\n",tokenString);
      break;
    case NE:      
      fprintf(listing,"!=\t\t\t%s\n",tokenString);
      break;
    case ASSIGN:  
      fprintf(listing,"=\t\t\t%s\n",tokenString);
      break;
    case SEMI:    
      fprintf(listing,";\t\t\t%s\n",tokenString);
      break;
    case COMMA:   
      fprintf(listing,",\t\t\t%s\n",tokenString);
      break;
    case LPAREN:   
      fprintf(listing,"(\t\t\t%s\n",tokenString);
      break;
    case RPAREN:   
      fprintf(listing,")\t\t\t%s\n",tokenString);
      break;
    case LBRACK:   
      fprintf(listing,"[\t\t\t%s\n",tokenString);
      break;
    case RBRACK:   
      fprintf(listing,"]\t\t\t%s\n",tokenString);
      break;
    case LBRACE:   
      fprintf(listing,"{\t\t\t%s\n",tokenString);
      break;
    case RBRACE:   
      fprintf(listing,"}\t\t\t%s\n",tokenString);
      break;
    default:    
           DONT_OCCUR_PRINT; 
  }
}

extern char program[100];
int printTab(map<function_name, function_Info> functions) {
	cout << "Code source name: " << program << endl;
	for (auto iter = functions.begin(); iter != functions.end(); iter++) {
		cout << "*******   Function Declaratioin Information     *******" << endl;
		cout << "Funciton name: " << iter->first << endl;
		//cout << "Declarate at line: " << iter->second.funcDecLineNo << endl;
		/*   now print params infomation    */
		cout << "Function's variables' infomation : " << endl;
		cout	<< setw(10) << setfill(' ') <<"DecLine"<<"       "\
				<< setw(10) << setfill(' ') << "Name" << "       " \
				<< setw(10) << setfill(' ') << "Type" << "       " \
				<< setw(10) << setfill(' ') << "ArrayLength" << "       " \
				<< setw(10) << setfill(' ') << "UsedLines" << endl;
		for (auto iter_param = iter->second.localVarInfo.begin(); iter_param != iter->second.localVarInfo.end(); iter_param++) {
			if (iter_param->second.isParam == true) {
				cout << setw(10) << setfill(' ') << iter_param->second.decLineNo << "       ";
				cout << setw(10) << setfill(' ') << iter_param->first << "       ";
				if (iter_param->second.type == MYINT) {
					cout << setw(10) << setfill(' ') << "int" << "       " << setw(10) << setfill(' ') << " "<< "       ";
				}
				else if (iter_param->second.type == MYARRAY) {
					cout << setw(10) << setfill(' ') <<"array"<< "       " << setw(10) << setfill(' ') << iter_param->second.length << "       ";
				}
				else {
					//now can not occur
				}
				/*print used lines*/
				for (int i = 0; i < iter_param->second.useLineNo.size(); i++) {
					cout << iter_param->second.useLineNo[i] << ',';
				}
				cout << endl;
			}
		}
		for (auto iter_param = iter->second.localVarInfo.begin(); iter_param != iter->second.localVarInfo.end(); iter_param++) {
			if (iter_param->second.isParam != true) {
				cout << setw(10) << setfill(' ') << iter_param->second.decLineNo << "       ";
				cout << setw(10) << setfill(' ') << iter_param->first << "       ";
				if (iter_param->second.type == MYINT) {
					cout << setw(10) << setfill(' ') << "int" << "       "<<setw(10) << setfill(' ') << " " << "       ";
				}
				else if (iter_param->second.type == MYARRAY) {
					cout << setw(10) << setfill(' ') << "array" << "       " << setw(10) << setfill(' ') << iter_param->second.length << "       ";
				}
				else {
					//now can not occur
				}
				/*print used lines*/
				for (int i = 0; i < iter_param->second.useLineNo.size(); i++) {
					cout << iter_param->second.useLineNo[i] << ',';
				}
				cout << endl;
			}
		}
		cout << endl;
		/*print functions are used in this function*/
		cout << "***   Functions are used in this function  ***" << endl;
		if (iter->second.used_funName.size() == 0) {
			cout << "     null       " << endl;
		}
		for (int i = 0; i < iter->second.used_funName.size(); i++) {
			cout << iter->second.used_funName[i] << "     ";
		}
		cout << "***   This function is used at WHERE  ***" << endl;
		cout << "Caller       LineNo" << endl;
		if (iter->second.beUsed.size() == 0) {
			cout << "     null       " << endl;
		}
		for (int i = 0; i < iter->second.beUsed.size(); i++) {
			cout << iter->second.beUsed[i].caller << "     " << iter->second.beUsed[i].lineno << endl;
		}
	}
}
