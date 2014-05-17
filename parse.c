#include "parse.h"

TreeNode * parse(void){
	TreeNode *ptr;
	yyparse((void *)ptr);
	return ptr;
}

void printTree(TreeNode *data){
	;
}
