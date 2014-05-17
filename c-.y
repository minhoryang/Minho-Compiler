/* Definition */
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define YYERROR_VERBOSE true
%}

%token <token> ENDFILE ERROR WARN
%token <token> ELSE IF INT RETURN VOID WHILE
%token <token> ID NUM
%token <token> PLUS MINUS TIMES OVER
%token <token> LESS_THAN LESS_EQUAL GREATER_THAN GREATER_EQUAL EQUAL NOT_EQUAL ASSIGN
%token <token> SEMI COMMA
%token <token> LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token <token> LEFT_BRACKET RIGHT_BRACKET
%token <token> LEFT_BRACE RIGHT_BRACE

%parse-param {void *ptr}

%union {
	struct type{
		int id;
		char *lexeme;
		int line;
	}token;
	struct{
		int id;
	};  // anonymous struct.  merge with above.
}

/* YACC Rule & Action */
%%
program : declaration_list
        ;
declaration_list : declaration_list declaration
				     {
					 }
                 | declaration
                     {
						 printf("h\n");
					 }
				 ;
declaration : var_declaration
            | fun_declaration
			;
var_declaration : type_specifier ID SEMI
                    {
						printf("%d %s\n", $2.id, $2.lexeme);
				    }
                | type_specifier ID LEFT_BRACKET NUM RIGHT_BRACKET SEMI
                    {
						printf("%d %s\n", $2.id, $2.lexeme);
						printf("%d %s\n", $4.id, $4.lexeme);
				    }
				;
type_specifier : INT
               | VOID
			   ;
fun_declaration : type_specifier ID LEFT_PARENTHESIS params RIGHT_PARENTHESIS compound_stmt
                    {
						printf("%d %s\n", $2.id, $2.lexeme);
					}
                ;
params : param_list
           {
		   }
       | VOID
	       {
		   }
	   ;
param_list : param_list COMMA param
               {
			   }
           | param
		       {
			   }
		   ;
param : type_specifier ID
	      {
						printf("%d %s\n", $2.id, $2.lexeme);
		  }
      | type_specifier ID LEFT_BRACKET RIGHT_BRACKET
	      {
						printf("%d %s\n", $2.id, $2.lexeme);
		  }
	  ;
compound_stmt : LEFT_BRACE local_declarations statement_list RIGHT_BRACE
                  {
				  }
              ;
local_declarations : local_declarations var_declaration
                       {
					   }
                   | /* empty */
			           {
				       }
				   ;
statement_list : statement_list statement
                   {
				   }
               | /* empty */
			       {
				   }
			   ;
statement : expression_stmt
              {
			  }
          | compound_stmt
              {
			  }
		  | selection_stmt
              {
			  }
		  | iteration_stmt
              {
			  }
		  | return_stmt
              {
			  }
		  ;
expression_stmt : expression SEMI
                    {
					}
                | SEMI
				    {
					}
				;
selection_stmt : IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement
			       {
				   }
               | IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement ELSE statement
			       {
				   }
			   ;
iteration_stmt : WHILE LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement
                   {
				   }
               ;
return_stmt : RETURN SEMI
                {
				}
            | RETURN expression SEMI
			    {
				}
			;
expression : var ASSIGN expression
               {
			   }
           | simple_expression
		       {
			   }
		   ;
var : ID
        {
						printf("%d %s\n", $1.id, $1.lexeme);
		}
    | ID LEFT_BRACKET expression RIGHT_BRACKET
	    {
						printf("%d %s\n", $1.id, $1.lexeme);
		}
	;
simple_expression : additive_expression relop additive_expression
                      {
					  }
                  | additive_expression
				      {
					  }
				  ;
relop : LESS_EQUAL
      | LESS_THAN
	  | GREATER_THAN
	  | GREATER_EQUAL
	  | EQUAL
	  | NOT_EQUAL
	  ;
additive_expression : additive_expression addop term
                        {
						}
                    | term
                        {
						}
					;
addop : PLUS
      | MINUS
	  ;
term : term mulop factor
         {
		 }
     | factor
	     {
		 }
	 ;
mulop : TIMES
      | OVER
	  ;
factor : LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
           {
		   }
       | var
           {
		   }
	   | call
           {
		   }
	   | NUM
           {
		   }
call : ID LEFT_PARENTHESIS args RIGHT_PARENTHESIS
         {
						printf("%d %s\n", $1.id, $1.lexeme);
		 }
     ;
args : arg_list
         {
		 }
     | /* empty */
	     {
		 }
	 ;
arg_list : arg_list COMMA expression
             {
			 }
         | expression
		     {
			 }
		 ;
%%
int yyerror(void *ptr, char *str){
	printf("%s\n", str);
	return 0;
}
