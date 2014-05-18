#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"

extern FILE *yyin;
extern FILE *yyout;

// XXX class naming.
typedef enum {
	/*program,*/
	declaration_list,
	/*_declaration,*/
	var_declaration,
	/*_type_specifier,*/
	fun_declaration,
	_params,
	param_list,
	param,
	compound_stmt,
	local_declarations,
	statement_list,
	_statement,
	expression_stmt,
	selection_stmt,
	iteration_stmt,
	return_stmt,
	expression,
	var,
	simple_expression,
	_relop,
	additive_expression,
	_addop,
	term,
	_mulop,
	factor,
	call,
	args,
	arg_list,
	num  // for factor.
} class;  // XXX funny naming. DO NOT DO THIS AT WORK :)

// XXX Structures
typedef struct _common{
	struct token;
	class type;
}_common_inherit;

typedef struct declaration_list{  // XXX done
	_common_inherit;
	List *list;
}Program;
typedef struct _declaration{  // XXX done
	_common_inherit;
	Elem elem;
	char *type_specifier;
	char *id;
}_declaration_inherit;
struct var_declaration{
	_declaration_inherit;
	char *size;
};
struct fun_declaration{
	_declaration_inherit;
	struct param_list* params;
};

struct param_list{
	_common_inherit;
	List *list;
};
typedef _declaration_inherit _param_inherit;
struct param{
	_param_inherit;
	bool isArray;
};

struct compound_stmt{
	_common_inherit;
	struct local_declarations *local_declarations;
	struct statement_list *statement_list;
};
struct local_declarations{
	_common_inherit;
	List *list;
};
struct statement_list{
	_common_inherit;
	List *list;
};
typedef struct _statement{
	_common_inherit;
	Elem elem;
}_statement_inherit;
struct expression_stmt{
	_statement_inherit;
	struct expression *expression;
};
struct selection_stmt{
	_statement_inherit;
	struct expression *condition;
	struct _statement *action;
	struct _statement *else_action;  // optional
};
struct iteration_stmt{
	_statement_inherit;
	struct expression *condition;
	struct _statement *action;
};
struct return_stmt{
	_statement_inherit;
	struct expression *return_expression;  // optional
};
struct expression{
	_common_inherit;
	bool isAssign;
	// true
	struct var *var;
	struct expression *expression;
	// false
	struct simple_expression *simple_expression;
};
struct var{
	_common_inherit;
	char *id;
	struct expression *array;  // optional
};
struct simple_expression{
	_common_inherit;
	struct additive_expression *left;
	// optional below
	char *relop;  // check this
		struct additive_expression *right;
};
struct additive_expression{
	_common_inherit;
		struct additive_expression *additive_expression;
	char *addop;  // check this
	// optional above
	struct term *term;
};
struct term{
	_common_inherit;
		struct term *term;
	char *mulop;  // check this
	// optional above
	struct factor *factor;
};
struct factor{
	_common_inherit;
	union {
		struct expression *expression;
		struct var *var;
		struct call *call;
		char *num;
	} link; 
	class linktype;  // check this
};


// XXX Func Defs
Program * parse(void);
void printTree(Program *);

Program *new_declaration_list();
struct var_declaration *new_var_declaration(char *type, char *id, char *size);
struct fun_declaration *new_fun_declaration(char *type, char *id, struct param_list *params);
struct param_list *new_param_list();
struct param *new_param(char *type_specifier, char *id, bool isArray);
struct compound_stmt *new_compound_stmt(struct local_declarations *ld, struct statement_list *st);
struct local_declarations *new_local_declarations();
struct statement_list *new_statement_list();
struct expression_stmt *new_expression_stmt(struct expression *expression);
struct selection_stmt *new_selection_stmt(
		struct expression *condition,
		struct _statement *action,
		struct _statement *else_action);
struct iteration_stmt *new_iteration_stmt(
		struct expression *condition,
		struct _statement *action);
struct return_stmt *new_return_stmt(struct expression *return_expression);
struct expression *new_expression_assign(
		struct var *var,
		struct expression *expression_);
struct expression *new_expression_simple(
		struct simple_expression *simple_expression);
struct var *new_var(char *id, struct expression *array);
struct simple_expression *new_simple_expression(
		struct additive_expression *left,
		char *relop,
		struct additive_expression *right);
struct additive_expression *new_additive_expression(
		struct additive_expression *additive_expression_,
		char *addop,
		struct term *term);
struct term *new_term(
		struct term *term_,
		char *mulop,
		struct factor *factor_);
struct factor *new_factor(struct token token);
