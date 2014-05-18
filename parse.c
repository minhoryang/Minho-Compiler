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

Program *new__declaration_list(){
	ALLOC(one, Program);
	one->list = _ALLOC(List);
	list_init(one->list);
	return one;
}

struct _var_declaration *new__var_declaration(char *type, char *id, char* size){
	ALLOC(one, struct _var_declaration);
	one->type_specifier = type;
	one->id = id;
	one->size = size;
	return one;
}
