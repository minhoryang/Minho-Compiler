#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "parse.h"

#ifndef my_symtab
#define my_symtab
struct symtab{
	Elem elem;
	Elem scopes_elem;
	List *symbols;
	List *usings;
	char *name;  // TODO parent?
};

void buildSymtab(Program *prog);
void typeCheck(Program *prog);
#endif
