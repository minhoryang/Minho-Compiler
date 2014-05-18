/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for CMINUS                     */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include <stdbool.h>
#include <string.h>
#include "globals.h"
#include "parse.h"

#define RETURN_LEXEME(ret) do{\
	yylval.token.lexeme = strdup(yytext); \
	yylval.token.id = ret; \
	yylval.token.line = lineno; \
	yylval.token.link = NULL; \
	return ret; \
}while(0);
%}

ID  {letter}+
NUM      {digit}+
letter      [a-zA-Z]
digit       [0-9]
newline     \n
whitespace  [ \t]+

%%

"else"	{RETURN_LEXEME(ELSE);}
"if"	{RETURN_LEXEME(IF);}
"int"	{RETURN_LEXEME(INT);}
"return"	{RETURN_LEXEME(RETURN);}
"void"	{RETURN_LEXEME(VOID);}
"while"	{RETURN_LEXEME(WHILE);}

"+"	{RETURN_LEXEME(PLUS);}
"-"	{RETURN_LEXEME(MINUS);}
"*"	{RETURN_LEXEME(TIMES);}
"/"	{RETURN_LEXEME(OVER);}
"<"	{RETURN_LEXEME(LESS_THAN);}
"<="	{RETURN_LEXEME(LESS_EQUAL);}
">"	{RETURN_LEXEME(GREATER_THAN);}
">="	{RETURN_LEXEME(GREATER_EQUAL);}
"=="	{RETURN_LEXEME(EQUAL);}
"!="	{RETURN_LEXEME(NOT_EQUAL);}
"="	{RETURN_LEXEME(ASSIGN);}
";"	{RETURN_LEXEME(SEMI);}
","	{RETURN_LEXEME(COMMA);}
"("	{RETURN_LEXEME(LEFT_PARENTHESIS);}
")"	{RETURN_LEXEME(RIGHT_PARENTHESIS);}
"["	{RETURN_LEXEME(LEFT_BRACKET);}
"]"	{RETURN_LEXEME(RIGHT_BRACKET);}
"{"	{RETURN_LEXEME(LEFT_BRACE);}
"}"	{RETURN_LEXEME(RIGHT_BRACE);}

{NUM}        {RETURN_LEXEME(NUM);}
{ID}    {RETURN_LEXEME(ID);}
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
							RETURN_LEXEME(ERROR);
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
					  RETURN_LEXEME(WARN);
                }

.               {RETURN_LEXEME(ERROR);}

%%

int yywrap(void){
	// http://dinosaur.compilertools.net/flex/flex_10.html
	/*
	 When the scanner receives an end-of-file indication from YY_INPUT, it then checks the `yywrap()' function. If `yywrap()' returns false (zero), then it is assumed that the function has gone ahead and set up yyin to point to another input file, and scanning continues. If it returns true (non-zero), then the scanner terminates, returning 0 to its caller. Note that in either case, the start condition remains unchanged; it does not revert to INITIAL.
	 If you do not supply your own version of `yywrap()', then you must either use `%option noyywrap' (in which case the scanner behaves as though `yywrap()' returned 1), or you must link with `-lfl' to obtain the default version of the routine, which always returns 1.
	 */
	return 1;
}
