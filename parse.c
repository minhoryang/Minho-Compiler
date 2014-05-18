#include "parse.h"
#define ALLOC(dest, type) type* dest = _ALLOC(type);
#define _ALLOC(type) (type *)calloc(1, sizeof(type));

Program * parse(void){
	Program *ptr;
	yyparse((void *)ptr);
	return ptr;
}

void printTree(Program *data){
	;
}

Program *new_declaration_list(){
	ALLOC(one, Program);
	one->list = _ALLOC(List);
	list_init(one->list);
	one->type = declaration_list;
	return one;
}

struct var_declaration *new_var_declaration(char *type, char *id, char *size){
	ALLOC(one, struct var_declaration);
	one->type_specifier = type;
	one->id = id;
	one->size = size;
	one->type = var_declaration;
	return one;
}

struct fun_declaration *new_fun_declaration(char *type, char *id, struct param_list *params){
	// TODO
	ALLOC(one, struct fun_declaration);
	one->type_specifier = type;
	one->id = id;
	one->params = params;
	one->type = fun_declaration;
	return one;
}

struct param_list *new_param_list(){
	ALLOC(one, struct param_list);
	one->list = _ALLOC(List);
	list_init(one->list);
	one->type = param_list;
	return one;
}

struct param *new_param(char *type_specifier, char *id, bool isArray){
	ALLOC(one, struct param);
	one->type_specifier = type_specifier;
	one->id = id;
	one->isArray = isArray;
	one->type = param;
	return one;
}

struct compound_stmt *new_compound_stmt(struct local_declarations *ld, struct statement_list *st){
	ALLOC(one, struct compound_stmt);
	one->type = compound_stmt;
	one->local_declarations = ld;
	one->statement_list = st;
	return one;
}

struct local_declarations *new_local_declarations(){
	ALLOC(one, struct local_declarations);
	one->type = local_declarations;
	one->list = _ALLOC(List);
	list_init(one->list);
	return one;
}

struct statement_list *new_statement_list(){
	ALLOC(one, struct statement_list);
	one->type = statement_list;
	one->list = _ALLOC(List);
	list_init(one->list);
	return one;
}
