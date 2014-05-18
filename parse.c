#include "parse.h"
#define ALLOC(dest, type) type* dest = _ALLOC(type);
#define _ALLOC(type) (type *)calloc(1, sizeof(type));

Program * parse(void){
	Program *ptr = NULL;
	yyparse(&ptr);
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
	one->name = id;
	one->size = size;
	one->type = var_declaration;
	return one;
}

struct fun_declaration *new_fun_declaration(char *type, char *id, struct param_list *params){
	// TODO
	ALLOC(one, struct fun_declaration);
	one->type_specifier = type;
	one->name = id;
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
	one->name = id;
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

struct expression_stmt *new_expression_stmt(struct expression *expression){
	ALLOC(one, struct expression_stmt);
	one->type = expression_stmt;
	one->expression = expression;
	return one;
}

struct selection_stmt *new_selection_stmt(
		struct expression *condition,
		struct _statement *action,
		struct _statement *else_action){
	ALLOC(one, struct selection_stmt);
	one->type = selection_stmt;
	one->condition = condition;
	one->action = action;
	one->else_action = else_action;
	return one;
}

struct iteration_stmt *new_iteration_stmt(
		struct expression *condition,
		struct _statement *action){
	ALLOC(one, struct iteration_stmt);
	one->type = iteration_stmt;
	one->condition = condition;
	one->action = action;
	return one;
}

struct return_stmt *new_return_stmt(struct expression *return_expression){
	ALLOC(one, struct return_stmt);
	one->type = return_stmt;
	one->return_expression = return_expression;
	return one;
}

struct expression *new_expression_assign(
		struct var *var,
		struct expression *expression_){
	ALLOC(one, struct expression);
	one->type = expression;
	one->isAssign = true;
	one->var = var;
	one->expression = expression_;
	return one;
}

struct expression *new_expression_simple(
		struct simple_expression *simple_expression){
	ALLOC(one, struct expression);
	one->type = expression;
	one->isAssign = false;
	one->simple_expression = simple_expression;
	return one;
}

struct var *new_var(char *id, struct expression *array){
	ALLOC(one, struct var);
	one->type = var;
	one->name = id;
	one->array = array;
	return one;
}

struct simple_expression *new_simple_expression(
		struct additive_expression *left,
		char *relop,
		struct additive_expression *right){
	ALLOC(one, struct simple_expression);
	one->type = simple_expression;
	one->left = left;
	one->relop = relop;
	one->right = right;
	return one;
}

struct additive_expression *new_additive_expression(
		struct additive_expression *additive_expression_,
		char *addop,
		struct term *term){
	ALLOC(one, struct additive_expression);
	one->type = additive_expression;
	one->additive_expression = additive_expression_;
	one->addop = addop;
	one->term = term;
	return one;
}

struct term *new_term(
		struct term *term_,
		char *mulop,
		struct factor *factor_){
	ALLOC(one, struct term);
	one->type = term;
	one->term = term_;
	one->mulop = mulop;
	one->factor = factor_;
	return one;
}

struct factor *new_factor(struct token token){
	ALLOC(one, struct factor);
	one->type = factor;
	switch(token.id){
		case LEFT_PARENTHESIS:
			{
				printf("WARN! @ %d\n", __LINE__);
			}
		case ID:
			{
				break;  // TODO until call().
				struct _common *target = (struct _common *)(token.link);
				switch(target->type){
					case expression:
					case var:
					case call:
						one->linktype = target->type;
						one->link = target;
						break;
				}
			}
			break;
		case NUM:
			{
				one->linktype = num;
				one->link = token.lexeme;
			}
			break;
	}
	return one;
}

struct call *new_call(char *id, struct arg_list *args){
	ALLOC(one, struct call);
	one->type = call;
	one->name = id;
	one->args = args;
	return one;
}

struct arg_list *new_arg_list(){
	ALLOC(one, struct arg_list);
	one->type = arg_list;
	one->list = _ALLOC(List);
	list_init(one->list);
	return one;
}
