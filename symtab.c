#include "symtab.h"

struct symtab * _allocSymtab(const char *name){
	struct symtab *st = _ALLOC(struct symtab);
	st->name = strdup(name);
	st->symbols = _ALLOC(List);
	list_init(st->symbols);
	st->usings = _ALLOC(List);
	list_init(st->usings);
	return st;
}

void _buildSymtab(struct _common *data, struct symtab *_context, bool func_exception){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				dl->symtab = _allocSymtab("Global");
				Elem *find_declaration;
				for(find_declaration = list_begin(dl->list);
					find_declaration != list_end(dl->list);
					find_declaration = list_next(find_declaration)){
						struct _declaration *this = list_entry(find_declaration, struct _declaration, elem);
						_buildSymtab((struct _common *)this, dl->symtab, false);
				}
			}
			break;
		case var_declaration:
			{
				struct var_declaration *vd = (struct var_declaration *)data;
				list_push_back(_context->symbols, &(vd->symelem));
			}
			break;
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				list_push_back(_context->symbols, &(fd->symelem));
				fd->symtab = _allocSymtab(fd->name);
				_buildSymtab((struct _common *)fd->params, fd->symtab, false);
				_buildSymtab((struct _common *)fd->compound_stmt, fd->symtab, true);
			}
			break;
		case param_list:
			{
				struct param_list *pl = (struct param_list *)data;
				Elem *find_param;
				for(find_param = list_begin(pl->list);
					find_param != list_end(pl->list);
					find_param = list_next(find_param)){
						struct param *this = list_entry(find_param, struct param, elem);
						_buildSymtab((struct _common *)this, _context, false);
				}
			}
			break;
		case param:
			{
				struct param *p = (struct param *)data;
				list_push_back(_context->symbols, &(p->symelem));
			}
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				struct symtab *context;
				if(func_exception){
					context = (cs->symtab = _allocSymtab(strcat(_context->name, "+c")));
				}else{
					context = _context;
				}
				_buildSymtab((struct _common *)cs->local_declarations, context, false);
				_buildSymtab((struct _common *)cs->statement_list, context, false);
			}
			break;
		case local_declarations:
			{
				struct local_declarations *ld = (struct local_declarations *)data;
				Elem *find_local_declarations;
				for(find_local_declarations = list_begin(ld->list);
					find_local_declarations != list_end(ld->list);
					find_local_declarations = list_next(find_local_declarations)){
						struct var_declaration *vd = list_entry(find_local_declarations, struct var_declaration, elem);
						_buildSymtab((struct _common *)vd, _context, false);
				}
			}
			break;
		case statement_list:
			{
				struct statement_list *sl = (struct statement_list *)data;
				Elem *find_statement_list;
				for(find_statement_list = list_begin(sl->list);
					find_statement_list != list_end(sl->list);
					find_statement_list = list_next(find_statement_list)){
						struct _statement *s = list_entry(find_statement_list, struct _statement, elem);
						_buildSymtab((struct _common *)s, _context, false);
				}
			}
			break;
		case expression_stmt:
			{
				struct expression *e = (struct expression *)data;
				if(e->expression)
					_buildSymtab((struct _common *)e->expression, _context, false);
			}
			break;
		case selection_stmt:
			{
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->condition)
					_buildSymtab((struct _common *)ss->condition, _context, false);
				if(ss->action)
					_buildSymtab((struct _common *)ss->action, _context, false);
				if(ss->else_action)
					_buildSymtab((struct _common *)ss->else_action, _context, false);
			}
			break;
		case iteration_stmt:
			{
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->condition)
					_buildSymtab((struct _common *)is->condition, _context, false);
				if(is->action)
					_buildSymtab((struct _common *)is->action, _context, false);
			}
			break;
		case return_stmt:
			{
				struct return_stmt *rs = (struct return_stmt *)data;
				if(rs->return_expression)
					_buildSymtab((struct _common *)rs->return_expression, _context, false);
			}
			break;
		case expression:
			{
				struct expression *e = (struct expression *)data;
				if(e->isAssign){
					if(e->var)
						_buildSymtab((struct _common *)e->var, _context, false);
					if(e->expression)
						_buildSymtab((struct _common *)e->expression, _context, false);
				}else{
					if(e->simple_expression)
						_buildSymtab((struct _common *)e->simple_expression, _context, false);
				}
			}
			break;
		case var:
			{
				struct var *v = (struct var *)data;
				list_push_back(_context->usings, &(v->symelem));
				if(v->array)
					_buildSymtab((struct _common *)v->array, _context, false);
			}
			break;
		case simple_expression:
			{
				struct simple_expression *se = (struct simple_expression *)data;
				if(se->left){
					_buildSymtab((struct _common *)se->left, _context, false);
				}
				if(se->relop){
					if(se->right){
						_buildSymtab((struct _common *)se->right, _context, false);
					}
				}
			}
			break;
		case additive_expression:
			{
				struct additive_expression *ae = (struct additive_expression *)data;
				if(ae->addop){
					if(ae->additive_expression){
						_buildSymtab((struct _common *)ae->additive_expression, _context, false);
					}
				}
				if(ae->term){
					_buildSymtab((struct _common *)ae->term, _context, false);
				}
			}
			break;
		case term:
			{
				struct term *t = (struct term *)data;
				if(t->mulop){
					if(t->term){
						_buildSymtab((struct _common *)t->term, _context, false);
					}
				}
				if(t->factor){
					_buildSymtab((struct _common *)t->factor, _context, false);
				}
			}
			break;
		case factor:
			{
				struct factor *f = (struct factor *)data;
				switch(f->linktype){
					case expression:
					case var:
					case call:
						_buildSymtab((struct _common *)f->link, _context, false);
						break;
					case num:
						break;
				}
			}
			break;
		case call:
			{
				struct call *c = (struct call *)data;
				if(c->args){
					_buildSymtab((struct _common *)c->args, _context, false);
				}
				list_push_back(_context->usings, &(c->symelem));
			}
			break;
		case arg_list:
			{
				struct arg_list *al = (struct arg_list *)data;
				Elem *find_arg_list;
				for(find_arg_list = list_begin(al->list);
					find_arg_list != list_end(al->list);
					find_arg_list = list_next(find_arg_list)){
						struct expression *e = list_entry(find_arg_list, struct expression, elem);
						_buildSymtab((struct _common *)e, _context, false);
				}
			}
			break;
		default:
			break;
	}
}

void buildSymtab(Program *prog){
	_buildSymtab((struct _common *)prog, NULL, false);
}

void typeCheck(Program *prog){
	;
}
