#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"

extern FILE *yyin;
extern FILE *yyout;

// XXX class naming.
typedef enum {
	/*program,*/
	_declaration_list,
	/*_declaration,*/
	var_declaration,
	/*_type_specifier,*/
	fun_declaration,
	_params,
	_param_list,
	_param,
	compound_stmt,
	local_declarations,
	statement_list,
	statement,
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

typedef struct _declaration_list{  // XXX done
	_common_inherit;
	List *list;
}Program;

typedef struct _declaration{  // XXX done
	_common_inherit;
	Elem elem;
}_declaration_inherit;

struct _var_declaration{
	_declaration_inherit;
	char *type_specifier;
	char *id;
	char *size;
};

struct _fun_declaration{
	_declaration_inherit;
};

// XXX Func Defs
Program * parse(void);
void printTree(Program *);

Program *new__declaration_list();
struct _var_declaration *new__var_declaration(char *type, char *id, char* size);
