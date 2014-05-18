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
	arg_list
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
struct _statement{
	_common_inherit;
	Elem elem;
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
