#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "list.h"

TreeNode * parse(void);
void printTree(TreeNode *);

extern FILE *yyin;
extern FILE *yyout;

typedef struct _common{
	//graph_type type;
	int type;
}_common;

struct _declaration_list{
	struct _common;
	List *declaration_s;
};

struct _var_declaration{
	_common;
};

struct _fun_declaration{
	_common;
};
