/* Definition */
%{
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parse.h"
#define YYERROR_VERBOSE true
#define INSERT(PARENT_T, PARENT, CHILD_T, CHILD) do{ \
	list_push_back( \
			( (PARENT_T *)(PARENT.link) )->list, \
			&( ( (CHILD_T *)(CHILD.link) )->elem ) ); \
}while(0);
#define INSERT2(PARENT_T, PARENT, CHILD_T, CHILD) do{ \
	((PARENT_T *)(PARENT.link))->list2[ \
		((PARENT_T *)(PARENT.link))->list2_l++ \
	] = (CHILD_T *)CHILD.link; \
}while(0);
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
%parse-param {Program **ptr}
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
%type <token> program declaration_list _declaration var_declaration _type_specifier fun_declaration _params param_list param compound_stmt local_declarations statement_list _statement expression_stmt selection_stmt iteration_stmt return_stmt expression var simple_expression _relop additive_expression _addop term _mulop factor call _args arg_list

/* YACC Rule & Action */
%%
program : declaration_list
            {
				*ptr = $1.link;
			}
        ;
declaration_list : declaration_list _declaration
				     {
						 $$ = $1;  // XXX merge.
						 {
							 struct declaration_list *args = (struct declaration_list *)($$.link);
							 List *list = args->list;
							 {
								 struct _declaration *exp = (struct _declaration *)($2.link);
								 list_push_back(list, &(exp->elem));
							 }
						 }
					 }
                 | _declaration
                     {
						 $$.link = new_declaration_list();
						 {
							 struct declaration_list *args = (struct declaration_list *)($$.link);
							 List *list = args->list;
							 {
								 struct _declaration *exp = (struct _declaration *)($1.link);
								 list_push_back(list, &(exp->elem));
							 }
						 }
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
						$$.link = new_fun_declaration($1.lexeme, $2.lexeme, $4.link, $6.link);
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
				   {
					   struct param_list *args = (struct param_list *)($$.link);
					   List *list = args->list;
					   {
						   struct param *exp = (struct param *)($3.link);
						   list_push_back(list, &(exp->elem));
					   }
				   }
			   }
           | param
		       {
				   $$.link = new_param_list();
				   {
					   struct param_list *args = (struct param_list *)($$.link);
					   List *list = args->list;
					   {
						   struct param *exp = (struct param *)($1.link);
						   list_push_back(list, &(exp->elem));
					   }
				   }
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
						   {
							   struct local_declarations *args = (struct local_declarations *)($$.link);
							   List *list = args->list;
							   {
								   struct var_declaration *exp = (struct var_declaration *)($2.link);
								   list_push_back(list, &(exp->elem));
							   }
						   }
					   }
                   | /* empty */
			           {
						   $$.link = new_local_declarations();
				       }
				   ;
statement_list : statement_list _statement
                   {
					   $$ = $1;
					   {
						   struct statement_list *args = (struct statement_list *)($$.link);
						   List *list = args->list;
						   {
							   struct _statement *exp = (struct _statement *)($2.link);
							   list_push_back(list, &(exp->elem));
						   }
					   }
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
						$$.link = new_expression_stmt($1.link);
					}
                | SEMI
				    {
						$$.link = new_expression_stmt(NULL);
					}
				;
selection_stmt : IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement
			       {
					   $$.link = new_selection_stmt($3.link, $5.link, NULL);
				   }
               | IF LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement ELSE _statement
			       {
					   $$.link = new_selection_stmt($3.link, $5.link, $7.link);
				   }
			   ;
iteration_stmt : WHILE LEFT_PARENTHESIS expression RIGHT_PARENTHESIS _statement
                   {
					   $$.link = new_iteration_stmt($3.link, $5.link);
				   }
               ;
return_stmt : RETURN SEMI
                {
					$$.link = new_return_stmt(NULL);
				}
            | RETURN expression SEMI
			    {
					$$.link = new_return_stmt($2.link);
				}
			;
expression : var ASSIGN expression
               {
				   $$.link = new_expression_assign($1.link, $3.link);
			   }
           | simple_expression
		       {
				   $$.link = new_expression_simple($1.link);
			   }
		   ;
var : ID
        {
			$$.link = new_var($1.lexeme, NULL);
		}
    | ID LEFT_BRACKET expression RIGHT_BRACKET
	    {
			$$.link = new_var($1.lexeme, $3.link);
		}
	;
simple_expression : additive_expression _relop additive_expression
                      {
						  $$.link = new_simple_expression($1.link, $2.lexeme, $3.link);
					  }
                  | additive_expression
				      {
						  $$.link = new_simple_expression($1.link, NULL, NULL);
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
							$$.link = new_additive_expression($1.link, $2.lexeme, $3.link);
						}
                    | term
                        {
							$$.link = new_additive_expression(NULL, NULL, $1.link);
						}
					;
_addop : PLUS
       | MINUS
	   ;
term : term _mulop factor
         {
			 $$.link = new_term($1.link, $2.lexeme, $3.link);
		 }
     | factor
	     {
			 $$.link = new_term(NULL, NULL, $1.link);
		 }
	 ;
_mulop : TIMES
       | OVER
	   ;
factor : LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
           {
			   $$.link = new_factor($2.link, expression);
		   }
       | var
           {
			   $$.link = new_factor($1.link, var);
		   }
	   | call
           {
			   $$.link = new_factor($1.link, call);
		   }
	   | NUM
           {
			   $$.link = new_factor($1.lexeme, num);
		   }
call : ID LEFT_PARENTHESIS _args RIGHT_PARENTHESIS
         {
			 $$.link = new_call($1.lexeme, $3.link);
		 }
     ;
_args : arg_list
      | /* empty */
	     {
			 $$.link = NULL;
		 }
	  ;
arg_list : arg_list COMMA expression
             {
				 $$ = $1;
				 {
					 struct arg_list *args = (struct arg_list *)($$.link);
					 List *list = args->list;
					 {
						 struct expression *exp = (struct expression *)($3.link);
						 list_push_back(list, &(exp->elem));
					 }
				 }
			 }
         | expression
		     {
				 $$.link = new_arg_list();
				 struct arg_list *args = (struct arg_list *)($$.link);
				 List *list = args->list;
				 {
					 struct expression *exp = (struct expression *)($1.link);
					 list_push_back(list, &(exp->elem));
				 }
			 }
		 ;
%%
int yyerror(Program **ptr, char *str){
	printf("%s\n", str);
	return 0;
}
