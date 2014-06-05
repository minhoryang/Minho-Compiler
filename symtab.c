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

void _buildSymtab(struct _common *data, struct symtab *_context, List *_scopes, bool func_exception){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				dl->symtab = _allocSymtab("Global");
				{
					_scopes = _ALLOC(List);
					list_init(_scopes);
					list_push_front(_scopes, &(dl->symtab->scopes_elem));
				}
				Elem *find_declaration;
				for(find_declaration = list_begin(dl->list);
					find_declaration != list_end(dl->list);
					find_declaration = list_next(find_declaration)){
						struct _declaration *this = list_entry(find_declaration, struct _declaration, elem);
						_buildSymtab((struct _common *)this, dl->symtab, _scopes, false);
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
				{
					ALLOC(scopes, List);
					list_init(scopes);
					list_push_front(scopes, &(fd->symtab->scopes_elem));
					{
						Elem *e;
						for(e = list_begin(_scopes);
							e != list_end(_scopes);
							e = list_next(e)){
								list_push_back(scopes, e);
								printf("%X\n", e);
						}
					}
					_scopes = scopes;
				}
				_buildSymtab((struct _common *)fd->params, fd->symtab, _scopes, false);
				_buildSymtab((struct _common *)fd->compound_stmt, fd->symtab, _scopes, true);
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
						_buildSymtab((struct _common *)this, _context, _scopes, false);
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
				if(!func_exception){
					context = (cs->symtab = _allocSymtab(strdup(_context->name)));
					{
						;  // TODO
					}
				}else{
					context = _context;
					cs->symtab = NULL;
				}
				_buildSymtab((struct _common *)cs->local_declarations, context, _scopes, false);
				_buildSymtab((struct _common *)cs->statement_list, context, _scopes, false);
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
						_buildSymtab((struct _common *)vd, _context, _scopes, false);
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
						_buildSymtab((struct _common *)s, _context, _scopes, false);
				}
			}
			break;
		case expression_stmt:
			{
				struct expression *e = (struct expression *)data;
				if(e->expression)
					_buildSymtab((struct _common *)e->expression, _context, _scopes, false);
			}
			break;
		case selection_stmt:
			{
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->condition)
					_buildSymtab((struct _common *)ss->condition, _context, _scopes, false);
				if(ss->action)
					_buildSymtab((struct _common *)ss->action, _context, _scopes, false);
				if(ss->else_action)
					_buildSymtab((struct _common *)ss->else_action, _context, _scopes, false);
			}
			break;
		case iteration_stmt:
			{
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->condition)
					_buildSymtab((struct _common *)is->condition, _context, _scopes, false);
				if(is->action)
					_buildSymtab((struct _common *)is->action, _context, _scopes, false);
			}
			break;
		case return_stmt:
			{
				struct return_stmt *rs = (struct return_stmt *)data;
				if(rs->return_expression)
					_buildSymtab((struct _common *)rs->return_expression, _context, _scopes, false);
			}
			break;
		case expression:
			{
				struct expression *e = (struct expression *)data;
				if(e->isAssign){
					if(e->var)
						_buildSymtab((struct _common *)e->var, _context, _scopes, false);
					if(e->expression)
						_buildSymtab((struct _common *)e->expression, _context, _scopes, false);
				}else{
					if(e->simple_expression)
						_buildSymtab((struct _common *)e->simple_expression, _context, _scopes, false);
				}
			}
			break;
		case var:
			{
				struct var *v = (struct var *)data;
				list_push_back(_context->usings, &(v->symelem));
				if(v->array)
					_buildSymtab((struct _common *)v->array, _context, _scopes, false);
			}
			break;
		case simple_expression:
			{
				struct simple_expression *se = (struct simple_expression *)data;
				if(se->left){
					_buildSymtab((struct _common *)se->left, _context, _scopes, false);
				}
				if(se->relop){
					if(se->right){
						_buildSymtab((struct _common *)se->right, _context, _scopes, false);
					}
				}
			}
			break;
		case additive_expression:
			{
				struct additive_expression *ae = (struct additive_expression *)data;
				if(ae->addop){
					if(ae->additive_expression){
						_buildSymtab((struct _common *)ae->additive_expression, _context, _scopes, false);
					}
				}
				if(ae->term){
					_buildSymtab((struct _common *)ae->term, _context, _scopes, false);
				}
			}
			break;
		case term:
			{
				struct term *t = (struct term *)data;
				if(t->mulop){
					if(t->term){
						_buildSymtab((struct _common *)t->term, _context, _scopes, false);
					}
				}
				if(t->factor){
					_buildSymtab((struct _common *)t->factor, _context, _scopes, false);
				}
			}
			break;
		case factor:
			{
				struct factor *f = (struct factor *)data;
				switch(f->contenttype){
					case expression:
						_buildSymtab((struct _common *)f->content.expression, _context, _scopes, false);
					case var:
						_buildSymtab((struct _common *)f->content.var, _context, _scopes, false);
					case call:
						_buildSymtab((struct _common *)f->content.call, _context, _scopes, false);
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
					_buildSymtab((struct _common *)c->args, _context, _scopes, false);
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
						_buildSymtab((struct _common *)e, _context, _scopes, false);
				}
			}
			break;
		default:
			break;
	}
}

#define TAPING do{ \
	int i; \
	for(i=0;i<level;i++) \
		printf("\t"); \
}while(false);
void _dumpSymtab(struct symtab *this, int level){
	// TODO list sort.
	Elem *find_symbols;
	for(find_symbols = list_begin(this->symbols);
		find_symbols != list_end(this->symbols);
		find_symbols = list_next(find_symbols)){
			struct _declaration *c = list_entry(find_symbols, struct _declaration, symelem);
			TAPING printf("%3d:%2d\t%s\t%s", c->line+1, c->cur+1, c->type_specifier, c->name);
			switch(c->type){
				case var_declaration:
					{
						struct var_declaration *vd = (struct var_declaration *)c;
						if(vd->size)
							printf("[%s]", vd->size);
					}
					break;
				case fun_declaration:
					{
						// TODO PARAM?
						printf("()");
					}
					break;
				case param:
					{
						struct param *p = (struct param *)c;
						if(p->isArray)
							printf("[]");
					}
					break;
			}
			printf(";\n");
	}
}

void _traceSymtab(struct _common *data, List *_tables, int level, int *cnt){
	if(data)
	switch(data->type){
		case declaration_list:
			{
				struct declaration_list *dl = (struct declaration_list *)data;
				{  // XXX IN!
					list_push_front(_tables, &(dl->symtab->elem));
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", dl->line+1, dl->cur+1, (*cnt)++, dl->symtab->name);
					level+=1;
					_dumpSymtab(dl->symtab, level);
				}
				Elem *find_declaration;
				for(find_declaration = list_begin(dl->list);
					find_declaration != list_end(dl->list);
					find_declaration = list_next(find_declaration)){
						struct _declaration *this = list_entry(find_declaration, struct _declaration, elem);
						_traceSymtab((struct _common *)this, _tables, level, cnt);
				}
				{	// XXX OUT!
					list_pop_front(_tables);
					level-=1;
				}
			}
			break;
		// var_declaration ignore.
		case fun_declaration:
			{
				struct fun_declaration *fd = (struct fun_declaration *)data;
				{	// XXX IN!
					list_push_front(_tables, &(fd->symtab->elem));
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", fd->line+1, fd->cur+1, (*cnt)++, fd->symtab->name);
					level+=1;
					_dumpSymtab(fd->symtab, level);
				}
				_traceSymtab((struct _common *)fd->compound_stmt, _tables, level, cnt);
				{	// XXX OUT!
					list_pop_front(_tables);
					level-=1;
				}
			}
			break;
		// param_list ignore.
		// param ignore.
		case compound_stmt:
			{
				struct compound_stmt *cs = (struct compound_stmt *)data;
				if(cs->symtab){  // XXX IN!
					list_push_front(_tables, &(cs->symtab->elem));
					TAPING printf("%3d:%2d\tSCOPE %d: \"%s\"\n", cs->line+1, cs->cur+1, (*cnt)++, cs->symtab->name);
					level+=1;
					_dumpSymtab(cs->symtab, level);
				}
				_traceSymtab((struct _common *)cs->statement_list, _tables, level, cnt);
				if(cs->symtab){  // XXX OUT!
					list_pop_front(_tables);
					level-=1;
				}
			}
			break;
		// local_declarations ignore.
		case statement_list:
			{
				struct statement_list *sl = (struct statement_list *)data;
				Elem *find_statement_list;
				for(find_statement_list = list_begin(sl->list);
					find_statement_list != list_end(sl->list);
					find_statement_list = list_next(find_statement_list)){
						struct _statement *s = list_entry(find_statement_list, struct _statement, elem);
						_traceSymtab((struct _common *)s, _tables, level, cnt);
				}
			}
			break;
		// expression_stmt ignore.
		case selection_stmt:
			{
				struct selection_stmt *ss = (struct selection_stmt *)data;
				if(ss->action)
					_traceSymtab((struct _common *)ss->action, _tables, level, cnt);
				if(ss->else_action)
					_traceSymtab((struct _common *)ss->else_action, _tables, level, cnt);
			}
			break;
		case iteration_stmt:
			{
				struct iteration_stmt *is = (struct iteration_stmt *)data;
				if(is->action)
					_traceSymtab((struct _common *)is->action, _tables, level, cnt);
			}
			break;
		// return_stmt ignore.
		// expression ignore.
		// var ignore.
		// simple_expression ignore.
		// additive_expression ignore.
		// term ignore.
		// factor ignore.
		// call ignore.
		// arg_list ignore.
		default:
			break;
	}
}

void buildSymtab(Program *prog){
	{
		List *_scopes;
		_buildSymtab((struct _common *)prog, NULL, _scopes, false);
	}
	if(TraceAnalyze){
		ALLOC(list, List);
		list_init(list);
		ALLOC(i, int);
		_traceSymtab((struct _common *)prog, list, 0, i);
		free(i);
		free(list);
	}
}

void typeCheck(Program *prog){
	;
}
