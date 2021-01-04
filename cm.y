%{
#define YYPARSER 

#include "treenodeAPI.h"
#include "SymbolList.h"

#define YYSTYPE TreeNode *

int Error = 1;
static TreeNode * savedTree; 

static int yylex(void);
int yyerror(char*);
%}

%token MINIMUM_TOKEN
%token ID NUM
%token ELSE IF INT RETURN VOID WHILE
%token PLUS MINUS TIMES OVER
%token LT LE GT GE EQ NE
%token ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token ENDFILE ERROR
%token MAXIMUM_TOKEN

%nonassoc LOWER_ELSE
%nonassoc ELSE

%start program

%% /* Grammar for C- */

program
        : declaration_list
          { savedTree = $1; }
        ;

declaration_list
        : declaration_list declaration
          { $$ = addSibling($1, $2); }
        | declaration
          { $$ = $1; }
        ;

declaration
        : var_declaration
          { $$ = $1; }
        | fun_declaration
          { $$ = $1; }
        ;

var_declaration
        : type_specifier _id SEMI
          { $$ = create_variable_node($1, $2); }
        | type_specifier _id LBRACK _num RBRACK SEMI
          { $$ = create_array_node($1, $2, $4); }
        ;

type_specifier
        : INT
          { $$ = create_token_node(INT); }
        | VOID
          { $$ = create_token_node(VOID); }

fun_declaration
        : type_specifier _id LPAREN params RPAREN compound_stmt
          { $$ = create_func_node($1, $2, $4, $6); }
        ;

params
        : param_list
          { $$ = $1; }
        | VOID
          { $$ = NULL; }
        ;

param_list
        : param_list COMMA param
          { $$ = addSibling($1, $3); }
        | param
          { $$ = $1; }
        ;

param
        : type_specifier _id
          { $$ = create_variable_param($1, $2); }
        | type_specifier _id LBRACK RBRACK
          { $$ = create_array_param($1, $2); }
        ;

compound_stmt
        : LBRACE local_declarations statement_list RBRACE
          { $$ = create_compound_sentence($2, $3); }
        ;

local_declarations
        : local_declarations var_declaration
          { $$ = addSibling($1, $2); }
        | /* empty */
          { $$ = NULL; }
        ;

statement_list
        : statement_list statement
          { $$ = addSibling($1, $2); }
        | /* empty */
          { $$ = NULL; }
        ;

statement
        : expression_stmt
          { $$ = $1; }
        | compound_stmt
          { $$ = $1; }
        | selection_stmt
          { $$ = $1; }
        | iteration_stmt
          { $$ = $1; }
        | return_stmt
          { $$ = $1; }
        ;

expression_stmt
        : expression SEMI
          { $$ = create_expression_sentence($1); }
        | SEMI
          { $$ = NULL; }
        ;

selection_stmt
        : IF LPAREN expression RPAREN statement
          { $$ = create_ifelse_sentence($3, $5, NULL); }
          %prec LOWER_ELSE
        | IF LPAREN expression RPAREN statement ELSE statement
          { $$ = create_ifelse_sentence($3, $5, $7); }
        ;

iteration_stmt
        : WHILE LPAREN expression RPAREN statement
          { $$ = create_while_sentence($3, $5); }
        ;

return_stmt
        : RETURN SEMI
          { $$ = create_return_sentence(NULL); }
        | RETURN expression SEMI
          { $$ = create_return_sentence($2); }
        ;

expression
        : var ASSIGN expression
         { $$ = create_assign_sentence($1, $3); }
        | simple_expression
         { $$ = $1; }
        ;

var
        : _id
          { $$ = $1; }
        | _id LBRACK expression RBRACK
          { $$ = create_array_sentence($1, $3); }
        ;

simple_expression
        : additive_expression relop additive_expression
          { $$ = create_relationop_sentence($1, $2, $3); }
        | additive_expression
          { $$ = $1; }
        ;

relop
        : LT
          { $$ = create_token_node(LT); }
        | LE
          { $$ = create_token_node(LE); }
        | GT
          { $$ = create_token_node(GT); }
        | GE
          { $$ = create_token_node(GE); }
        | EQ
          { $$ = create_token_node(EQ); }
        | NE
          { $$ = create_token_node(NE); }
        ;

additive_expression
        : additive_expression addop term
          { $$ = create_addsubop_sentence($1, $2, $3); }
        | term
          { $$ = $1; }
        ;

addop
        : PLUS
          { $$ = create_token_node(PLUS); }
        | MINUS
          { $$ = create_token_node(MINUS); }

term
        : term mulop factor
          { $$ = create_muldivop_sentence($1, $2, $3); }
        | factor
          { $$ = $1; }
        ;

mulop
        : TIMES
          { $$ = create_token_node(TIMES); }
        | OVER
          { $$ = create_token_node(OVER); }
        ;

factor
        : LPAREN expression RPAREN
          { $$ = $2; }
        | var
          { $$ = $1; }
        | call
          { $$ = $1; }
        | _num
          { $$ = $1; }
        ;

call
        : _id LPAREN args RPAREN
          { $$ = create_call_sentence($1, $3); }
        ;

args
        : arg_list
          { $$ = $1; }
        | /* empty */
          { $$ = NULL; }
        ;

arg_list
        : arg_list COMMA expression
          { $$ = addSibling($1, $3); }
        | expression
          { $$ = $1; }
        ;

_id
        : ID
          { $$ = create_ID_node(tokenString); }
        ;

_num
        : NUM
          { $$ = create_NUM_node(tokenString); }
        ;

%%

int yyerror(char * message)
{
  fprintf(listing,"Syntax error at line %d: %s\n",lineno,message);
  fprintf(listing, "Current token: ");
  printToken(yychar,tokenString);
  Error = TRUE;
  return 0;
}

static int yylex(void)
{
  TokenType tok = getToken();
  if (tok == ENDFILE) return 0;
  if (tok == ERROR)
    {
      fprintf(listing,"Lexical analyze error at line %d\n",lineno);
      fprintf(listing,"Current token: %s",tokenString);
      Error = TRUE;
      return 0;
    }
  return tok;
}

TreeNode * parse(void)
{
  yyparse();
  return savedTree;
}

