/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for CMINUS                     */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include <stdbool.h>
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

"/*"             { char c; bool assume_comment = false, assume_warn = false, real_warn = false;
                  do
                  { c = input();
					if(assume_comment){
						if(c == '/'){
							break;
						}else{
							assume_comment = false;
						}
					}else if(assume_warn){
						if(c == '*'){
							real_warn = true;
						}else{
							assume_warn = false;
						}
					}
					switch(c){
						case EOF:
							return ERROR;
						case '\n':
							lineno++;
							break;
						case '*':
							assume_comment = true;
							break;
						case '/':
							assume_warn = true;
							break;
						default:
							break;
					}
                  } while (true);
				  if(real_warn)
					  return WARN;
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

int yywrap(void){
	// http://dinosaur.compilertools.net/flex/flex_10.html
	/*
	 When the scanner receives an end-of-file indication from YY_INPUT, it then checks the `yywrap()' function. If `yywrap()' returns false (zero), then it is assumed that the function has gone ahead and set up yyin to point to another input file, and scanning continues. If it returns true (non-zero), then the scanner terminates, returning 0 to its caller. Note that in either case, the start condition remains unchanged; it does not revert to INITIAL.
	 If you do not supply your own version of `yywrap()', then you must either use `%option noyywrap' (in which case the scanner behaves as though `yywrap()' returned 1), or you must link with `-lfl' to obtain the default version of the routine, which always returns 1.
	 */
	return 1;
}
