#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "parse.h"

#ifndef my_symtab
#define my_symtab
struct symtab{
	Elem elem;
	List *symbols;
	List *usings;
	char *name;  // TODO parent?
	struct symtab *parent;
};

void buildSymtab(Program *prog);
void typeCheck(Program *prog);

struct symtab *searchSymtabWhere(struct symtab *from, char *this);
struct _symbol_common *searchSymtab(struct symtab *from, char *this);
#endif
