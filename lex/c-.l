/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for CMINUS                     */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of ID or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

ID  {letter}+
NUM      {digit}+
letter      [a-zA-Z]
digit       [0-9]
newline     \n
whitespace  [ \t]+

%%

"else"	{return ELSE;}
"if"	{return IF;}
"int"	{return INT;}
"return"	{return RETURN;}
"void"	{return VOID;}
"while"	{return WHILE;}

"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return TIMES;}
"/"	{return OVER;}
"<"	{return LESS_THAN;}
"<="	{return LESS_EQUAL;}
">"	{return GREATER_THAN;}
">="	{return GREATER_EQUAL;}
"=="	{return EQUAL;}
"!="	{return NOT_EQUAL;}
"="	{return ASSIGN;}
";"	{return SEMI;}
","	{return COMMA;}
"("	{return LEFT_PARENTHESIS;}
")"	{return RIGHT_PARENTHESIS;}
"["	{return LEFT_BRACKET;}
"]"	{return RIGHT_BRACKET;}
"{"	{return LEFT_BRACE;}
"}"	{return RIGHT_BRACE;}

{NUM}        {return NUM;}
{ID}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}

"/*"             { char c;
                  do
                  { c = input();
                    if (c == EOF) break;
                    if (c == '\n') lineno++;
                  } while (c != '}');
                }

.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

