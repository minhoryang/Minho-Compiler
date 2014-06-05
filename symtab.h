#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parse.h"

#ifndef my_symtab
#define my_symtab
struct symtab{
	struct symtab *parent;
	Elem elem;
	List *symbols;
	char d[10];
	List *usings;
	char d2[10];
	char *name;  // TODO parent?
};

void buildSymtab(Program *prog);
void typeCheck(Program *prog);
struct symtab *searchSymtabWhere(struct symtab *from, char *this);
struct _common *searchSymtab(struct symtab *from, char *this);
#endif
