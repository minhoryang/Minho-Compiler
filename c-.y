/* Definition */
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parse.h"
#define YYERROR_VERBOSE true
#define LINK(target) (_common *)(target.link)
#define COPY(target, destination) do{ \
}while(0);
#define INSERT(PARENT_T, PARENT, CHILD_T, CHILD) do{ \
	list_push_back( \
			( (PARENT_T *)(PARENT.link) )->list, \
			&( ( (CHILD_T *)(&(CHILD)) )->elem) ); \
}while(0);
#define TYPE(target, name) ((_common_inherit *)(target.link))->type = name;
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
%parse-param {Program *ptr}
%union {
	struct token{
		int id;
		char *lexeme;
		int line;
		void *link;
	}token;
	struct{
		int id;  // XXX just for return();
	};  // XXX (anonymous struct) merge with above.
}
%type <token> program declaration_list _declaration var_declaration _type_specifier fun_declaration _params param_list param compound_stmt local_declarations statement_list _statement expression_stmt selection_stmt iteration_stmt return_stmt expression var simple_expression _relop additive_expression _addop term _mulop factor call args arg_list

/* YACC Rule & Action */
%%
program : declaration_list
            {
				ptr = $1.link;
				//printf("Program: <%X>\n", ptr);
			}
        ;
declaration_list : declaration_list _declaration
				     {
						 $$ = $1;  // XXX merge.
						 INSERT(struct declaration_list, $$, _declaration_inherit, $2);
					 }
                 | _declaration
                     {
						 $$.link = new_declaration_list();
						 INSERT(struct declaration_list, $$, _declaration_inherit, $1);
					 }
				 ;
_declaration : var_declaration
             | fun_declaration
	         ;
var_declaration : _type_specifier ID SEMI
                    {
						$$.link = new_var_declaration($1.lexeme, $2.lexeme, NULL);
				    }
                | _type_specifier ID LEFT_BRACKET NUM RIGHT_BRACKET SEMI
                    {
						$$.link = new_var_declaration($1.lexeme, $2.lexeme, $4.lexeme);
				    }
				;
_type_specifier : INT
                | VOID
			    ;
fun_declaration : _type_specifier ID LEFT_PARENTHESIS _params RIGHT_PARENTHESIS compound_stmt
                    {
						// TODO
						$$.link = new_fun_declaration($1.lexeme, $2.lexeme, $4.link);
					}
                ;
_params : param_list
           {
			   $$ = $1;
		   }
        | VOID
	       {
			   $$.link = NULL;
		   }
	    ;
param_list : param_list COMMA param
               {
				   $$ = $1;
				   INSERT(struct param_list, $$, struct param, $3);
			   }
           | param
		       {
				   $$.link = new_param_list();
				   INSERT(struct param_list, $$, struct param, $1);
			   }
		   ;
param : _type_specifier ID
	      {
			  $$.link = new_param($1.lexeme, $2.lexeme, false);
		  }
       | _type_specifier ID LEFT_BRACKET RIGHT_BRACKET
	      {
			  $$.link = new_param($1.lexeme, $2.lexeme, true);
		  }
	   ;
compound_stmt : LEFT_BRACE local_declarations statement_list RIGHT_BRACE
                  {
					  $$.link = new_compound_stmt($2.link, $3.link);
				  }
              ;
local_declarations : local_declarations var_declaration
                       {
						   $$ = $1;
						   // INSERT(struct local_declarations, $$, struct var_declaration, $2);
					   }
                   | /* empty */
			           {
						   $$.link = new_local_declarations();
				       }
				   ;
statement_list : statement_list _statement
                   {
					   $$ = $1;
					   // INSERT(struct statement_list, $$, struct statement, $2);
				   }
               | /* empty */
			       {
					   $$.link = new_statement_list();
				   }
			   ;
_statement : expression_stmt
          | compound_stmt
		  | selection_stmt
		  | iteration_stmt
		  | return_stmt
		  ;
expression_stmt : expression SEMI
                    {
					}
                | SEMI
				    {
					}
				;
selection_stmt : IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement
			       {
				   }
               | IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement ELSE _statement
			       {
				   }
			   ;
iteration_stmt : WHILE LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement
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
simple_expression : additive_expression _relop additive_expression
                      {
					  }
                  | additive_expression
				      {
					  }
				  ;
_relop : LESS_EQUAL
       | LESS_THAN
	   | GREATER_THAN
 	   | GREATER_EQUAL
 	   | EQUAL
	   | NOT_EQUAL
	   ;
additive_expression : additive_expression _addop term
                        {
						}
                    | term
                        {
						}
					;
_addop : PLUS
       | MINUS
	   ;
term : term _mulop factor
         {
		 }
     | factor
	     {
		 }
	 ;
_mulop : TIMES
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
int yyerror(Program *ptr, char *str){
	printf("%s\n", str);
	return 0;
}
