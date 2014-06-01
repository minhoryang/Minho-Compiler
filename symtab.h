#include "parse.h"

struct symtab{
	List *symbols;
	List *usings;
	char *name;  // TODO parent?
};
