%option noyywrap
%option noinput
%option nounput

%{
#include "treenodeAPI.h"
#include "SymbolList.h"
char tokenString[MAXTOKENLEN+1];
%}

%x COMMENT

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  [a-zA-Z][a-zA-Z0-9]*
newline     \n
whitespace  [ \t]+

%%


"else"              { return ELSE; }
"if"                { return IF; }
"int"               { return INT; }
"return"            { return RETURN; }
"void"              { return VOID; }
"while"             { return WHILE; }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return TIMES; }
"/"                 { return OVER; }

"<"                 { return LT; }
"<="                { return LE; }
">"                 { return GT; }
">="                { return GE; }
"=="                { return EQ; }
"!="                { return NE; }

"="                 { return ASSIGN; }
";"                 { return SEMI; }
","                 { return COMMA; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"["                 { return LBRACK; }
"]"                 { return RBRACK; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }

{number}            { return NUM; }
{identifier}        { return ID; }
{newline}           { lineno++; }
{whitespace}        { /* skip whitespace */ }

"/*"                { BEGIN(COMMENT); }
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>.          { /* skip comments */ }
<COMMENT>{newline}  { lineno++; }
<COMMENT><<EOF>>    { BEGIN(INITIAL); return ERROR; }
"*/"                { return ERROR; }
<<EOF>>             { return ENDFILE; }
%%

TokenType
getToken(void)
{
  static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
    {
      firstTime = FALSE;
      lineno++;
      yyin = source;
      yyout = listing;
    }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan)
    {
      fprintf(listing,"\t%d\t\t\t",lineno);
      printToken(currentToken,tokenString);
    }
  return currentToken;
}
